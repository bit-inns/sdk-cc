#ifndef CRYPTO_MACRO_H
#define CRYPTO_MACRO_H

#include <cstdint>


namespace crypto::aes
{


enum class aes_set
{
  aes128,
  aes192,
  aes256,
};

inline constexpr unsigned int aes_block_len = 16;

inline constexpr unsigned int aes128_key_len = 16;
inline constexpr unsigned int aes192_key_len = 24;
inline constexpr unsigned int aes256_key_len = 32;

inline constexpr unsigned int aes128_rounds = 10;
inline constexpr unsigned int aes192_rounds = 12;
inline constexpr unsigned int aes256_rounds = 14;
inline constexpr unsigned int aes_max_rounds = 14;

inline constexpr unsigned int aes_round_key_len = 16;
inline constexpr unsigned int aes_key_word_len = 4;


} // namespace crypto::aes


namespace crypto::sha
{


inline constexpr unsigned int seed_len = 32;

inline constexpr unsigned int hash128_len = 16;
inline constexpr unsigned int hash256_len = 32;
inline constexpr unsigned int hash512_len = 64;


enum class shake_set
{
  shake128,
  shake256,
};

inline constexpr unsigned int keccak_rounds = 24;

/// keccak-f[1600] permutation uses a 1600-bit (200-byte) unsigned internal state
/// r = 1344 bits / 168 bytes, c = 256 bits
inline constexpr unsigned int shake128_rate = 168;
inline constexpr unsigned int shake256_rate = 136;

inline constexpr uint8_t shake_suffix = 0x1f;


enum class sha3_set
{
  sha3_256,
  sha3_512,
};

inline constexpr unsigned int sha3_256_rate = 136;
inline constexpr unsigned int sha3_512_rate = 72;

inline constexpr uint8_t sha3_suffix = 0x06;


} // namespace crypto::sha


namespace crypto::mlkem
{


inline constexpr unsigned int seed_len = 32;

enum class mlkem_set
{
  mlkem_512,
  mlkem_768,
  mlkem_1024,
};

/// mlkem operates in ring R_q: Z_q^n
inline constexpr unsigned int mlkem_q = 3329;

/// when using montgomery reduction, need q^{-1} mod 2^16
inline constexpr unsigned int mlkem_inverse_q = -3327;

/// number of coefficients in a polynomial
inline constexpr unsigned int mlkem_n = 256;

/// number of poly in a vector of polys
inline constexpr unsigned int mlkem512_k = 2;
inline constexpr unsigned int mlkem768_k = 3;
inline constexpr unsigned int mlkem1024_k = 4;
inline constexpr unsigned int mlkem_max_k = mlkem1024_k;

inline constexpr unsigned int mlkem512_eta1 = 3;
inline constexpr unsigned int mlkem768_eta1 = 2;
inline constexpr unsigned int mlkem1024_eta1 = 2;
inline constexpr unsigned int mlkem_max_eta1 = mlkem512_eta1;

inline constexpr unsigned int mlkem_eta2 = 2;

/// polynomial byte len: 12 bits/coefficient, 256 * 12 / 8 = 384
inline constexpr unsigned int mlkem_poly_len = 384;

inline constexpr unsigned int mlkem512_poly_vec_len = mlkem512_k * mlkem_poly_len;
inline constexpr unsigned int mlkem768_poly_vec_len = mlkem768_k * mlkem_poly_len;
inline constexpr unsigned int mlkem1024_poly_vec_len = mlkem1024_k * mlkem_poly_len;

inline constexpr unsigned int mlkem512_compressed_poly_len = 128;
inline constexpr unsigned int mlkem768_compressed_poly_len = 128;
inline constexpr unsigned int mlkem1024_compressed_poly_len = 160;
inline constexpr unsigned int mlkem_max_compressed_poly_len = mlkem1024_compressed_poly_len;

inline constexpr unsigned int mlkem512_compressed_poly_vec_len = mlkem512_k * 320;
inline constexpr unsigned int mlkem768_compressed_poly_vec_len = mlkem768_k * 320;
inline constexpr unsigned int mlkem1024_compressed_poly_vec_len = mlkem1024_k * 352;
inline constexpr unsigned int mlkem_max_compressed_poly_vec_len = mlkem1024_compressed_poly_vec_len;

inline constexpr unsigned int mlkem_msg_len = 32;

inline constexpr unsigned int mlkem512_ek_len = mlkem512_poly_vec_len + seed_len;
inline constexpr unsigned int mlkem768_ek_len = mlkem768_poly_vec_len + seed_len;
inline constexpr unsigned int mlkem1024_ek_len = mlkem1024_poly_vec_len + seed_len;

inline constexpr unsigned int mlkem512_dk_len = mlkem512_poly_vec_len * 2 + seed_len * 2 + sha::hash256_len;
inline constexpr unsigned int mlkem768_dk_len = mlkem768_poly_vec_len * 2 + seed_len * 2 + sha::hash256_len;
inline constexpr unsigned int mlkem1024_dk_len = mlkem1024_poly_vec_len * 2 + seed_len * 2 + sha::hash256_len;


} // namespace crypto::mlkem


#endif // #ifndef CRYPTO_MACRO_H