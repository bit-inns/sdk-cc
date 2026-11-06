#ifndef SDK_CRYPTO_MLKEM_SHAKE_H
#define SDK_CRYPTO_MLKEM_SHAKE_H

#include <cstddef>
#include <cstdint>

#include "sdk/crypto/sha/sha3.h"


namespace sdk::crypto::mlkem
{


struct mlkem_ctx;

/**
 * Generate random bytes as needed to fill the polynomial coefficients in matrix.
 */
void shake128_absorb(sha::keccak_ctx *ctx, const uint8_t seed[seed_len], uint8_t x, uint8_t y);

/**
 * Generate the noise polynomials and secret vectors
 */
void shake256_prf(uint8_t *out, size_t len, const uint8_t key[seed_len], uint8_t nonce);

/**
 * Regenerate final shared secret after the ciphertext has been processed
 * @param out shared secret
 * @param key part of mlkem private key
 * @param input actual cipher text received over the network, len: compressed_poly_vec_len+compressed_poly_len
 */
void shake256_rkprf(
  const mlkem_ctx *mlkem_ctx, uint8_t out[sha::hash256_len], const uint8_t key[seed_len], const uint8_t *input
);


} // namespace sdk::crypto::mlkem


#endif // #ifndef SDK_CRYPTO_MLKEM_SHAKE_H