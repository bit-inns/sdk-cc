#ifndef SDK_CRYPTO_MODE_CBC_H
#define SDK_CRYPTO_MODE_CBC_H

#include <array>
#include <cstddef>
#include <cstdint>


namespace sdk::crypto::mode
{


/**
 * @brief Encrypt whole blocks using cipher block chaining (CBC), without padding.
 * @tparam BlockSize Block size in bytes; must be greater than zero.
 * @tparam EncryptBlock Callable accepting (uint8_t *out, const uint8_t *in).
 * @param encrypt_block Encrypts exactly BlockSize bytes with the caller's configured key.
 *        Input and output passed to this callable are distinct, unaligned byte buffers.
 * @param out Destination with space for len bytes.
 * @param in Source containing len bytes. Exact in-place operation (out == in) is supported;
 *        otherwise the source and destination must not overlap.
 * @param len Number of bytes; must be a multiple of BlockSize.
 * @param iv Writable BlockSize-byte chaining value, separate from in and out. On success,
 *        updated to the last ciphertext block. Use a fresh, unpredictable IV for each new message.
 * @return true on success, false for an invalid length or a null buffer when len is nonzero.
 *         On false, no buffers are changed and the cipher is not called.
 * @note Zero length succeeds without accessing buffers or calling the cipher.
 * @note CBC provides no authentication. Padding and message authentication belong to the caller.
 * @note The callable must write one complete block and signal failure by throwing; exceptions propagate.
 *       If it throws, output and IV may already contain a processed prefix.
 */
template<std::size_t BlockSize, typename EncryptBlock>
bool cbc_encrypt(EncryptBlock &&encrypt_block, uint8_t *out, const uint8_t *in, std::size_t len, uint8_t *iv) {
  static_assert(BlockSize > 0, "CBC requires a nonzero block size");
  if (len % BlockSize != 0) return false;
  if (len == 0) return true;
  if (!out || !in || !iv) return false;

  std::array<uint8_t, BlockSize> block;
  for (std::size_t offset = 0; offset < len; offset += BlockSize) {
    for (std::size_t i = 0; i < BlockSize; ++i) {
      block[i] = in[offset+i] ^ iv[i];
    }
    encrypt_block(out+offset, block.data());
    for (std::size_t i = 0; i < BlockSize; ++i) {
      iv[i] = out[offset+i];
    }
  }
  return true;
}

/**
 * @brief Decrypt whole blocks using cipher block chaining (CBC), without removing padding.
 * @tparam BlockSize Block size in bytes; must be greater than zero.
 * @tparam DecryptBlock Callable accepting (uint8_t *out, const uint8_t *in).
 * @param decrypt_block Decrypts exactly BlockSize bytes with the caller's configured key.
 *        Input and output passed to this callable are distinct, unaligned byte buffers.
 * @param out Destination with space for len bytes.
 * @param in Ciphertext containing len bytes. Exact in-place operation (out == in) is supported;
 *        otherwise the source and destination must not overlap.
 * @param len Number of bytes; must be a multiple of BlockSize.
 * @param iv Writable BlockSize-byte chaining value, separate from in and out. Initially the
 *        encryption IV; on success, updated to the last input ciphertext block.
 * @return true on success, false for an invalid length or a null buffer when len is nonzero.
 *         On false, no buffers are changed and the cipher is not called.
 * @note Zero length succeeds without accessing buffers or calling the cipher.
 * @note Authenticate ciphertext before using decrypted data. This function does not validate padding.
 * @note The callable must write one complete block and signal failure by throwing; exceptions propagate.
 *       If it throws, output and IV may already contain a processed prefix.
 */
template<std::size_t BlockSize, typename DecryptBlock>
bool cbc_decrypt(DecryptBlock &&decrypt_block, uint8_t *out, const uint8_t *in, std::size_t len, uint8_t *iv) {
  static_assert(BlockSize > 0, "CBC requires a nonzero block size");
  if (len % BlockSize != 0) return false;
  if (len == 0) return true;
  if (!out || !in || !iv) return false;

  std::array<uint8_t, BlockSize> cipher;
  std::array<uint8_t, BlockSize> block;
  for (std::size_t offset = 0; offset < len; offset += BlockSize) {
    for (std::size_t i = 0; i < BlockSize; ++i) {
      cipher[i] = in[offset+i];
    }
    decrypt_block(block.data(), cipher.data());
    for (std::size_t i = 0; i < BlockSize; ++i) {
      out[offset+i] = block[i] ^ iv[i];
      iv[i] = cipher[i];
    }
  }
  return true;
}


} // namespace sdk::crypto::mode


#endif // #ifndef SDK_CRYPTO_MODE_CBC_H