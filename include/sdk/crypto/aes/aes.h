#ifndef SDK_CRYPTO_AES_AES_H
#define SDK_CRYPTO_AES_AES_H

#include <cstdint>
#include <cstring>

#include "sdk/crypto/macro.h"


namespace sdk::crypto::aes
{


struct aes_ctx
{
  uint8_t round_keys_[aes_max_rounds+1][aes_round_key_len];
  unsigned int nr_;
  unsigned int kl_;
  uint64_t state_[aes_block_len/sizeof(uint64_t)];
  static_assert(aes_block_len/sizeof(uint64_t)==2, "sdk::crypto : aes error");
};


/** 
 * Encrypt one 16-byte block; in-place operation is supported.
 */
void encrypt(aes_set set, const uint8_t *key, uint8_t out[aes_block_len], const uint8_t in[aes_block_len]) noexcept;

/** 
 * Decrypt one 16-byte block; in-place operation is supported.
 */
void decrypt(aes_set set, const uint8_t *key, uint8_t out[aes_block_len], const uint8_t in[aes_block_len]) noexcept;


} // namespace sdk::crypto::aes


#endif // #ifndef SDK_CRYPTO_AES_AES_H