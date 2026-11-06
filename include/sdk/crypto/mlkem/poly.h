#ifndef SDK_CRYPTO_MLKEM_POLY_H
#define SDK_CRYPTO_MLKEM_POLY_H

#include <cstdint>

#include "sdk/crypto/ctops.h"
#include "sdk/crypto/macro.h"
#include "sdk/crypto/mlkem/ntt.h"


namespace sdk::crypto::mlkem
{


struct mlkem_ctx;


/// A polynomial whose coefficient is roughly 12 bits in Z_3329
struct poly
{
  int16_t coeffs[256];
};

/**
 * Pack 2 12-bits cofficients of a polynomial to 3 bytes
 */
void poly_to_bytes(uint8_t out[mlkem_poly_len], const poly *p);

/**
 * Unpack 3 bytes to 2 12-bits coefficients of a polynomial
 */
void bytes_to_poly(poly *p, const uint8_t in[mlkem_poly_len]);

void poly_reduce(poly *p);

void poly_tomont(poly *p);

void poly_ntt(poly *p);

void poly_invntt(poly *p);

void poly_add(poly *p, const poly *a, const poly *b);

void poly_sub(poly *p, const poly *a, const poly *b);

void poly_basemul(poly *p, const poly *a, const poly *b);

/**
 * Compress the polynomial into an array of bytes, compress(x, d) = floor( (2^d/mlkem_q)*x + 1/2 )
 */
void poly_compress(const mlkem_ctx *ctx, const poly *p, uint8_t *out);

void poly_decompress(const mlkem_ctx *ctx, const uint8_t *in, poly *p);

void poly_to_msg(uint8_t m[mlkem_msg_len], const poly *p);

void msg_to_poly(poly *p, const uint8_t m[mlkem_msg_len]);


struct poly_vec
{
  poly vec[mlkem_max_k];
};

void poly_vec_to_bytes(const mlkem_ctx *ctx, const poly_vec *pv, uint8_t *out);

void bytes_to_poly_vec(const mlkem_ctx *ctx, const uint8_t *in, poly_vec *pv);

void poly_vec_reduce(const mlkem_ctx *ctx, poly_vec *pv);

void poly_vec_ntt(const mlkem_ctx *ctx, poly_vec *pv);

void poly_vec_invntt(const mlkem_ctx *ctx, poly_vec *pv);

void poly_vec_add(const mlkem_ctx *ctx, poly_vec *pv, const poly_vec *a, const poly_vec *b);

void poly_vec_basemul(const mlkem_ctx *ctx, poly *p, const poly_vec *a, const poly_vec *b);

void poly_vec_compress(const mlkem_ctx *ctx, const poly_vec *p, uint8_t *out);

void poly_vec_decompress(const mlkem_ctx *ctx, const uint8_t *in, poly_vec *p);


} // namespace sdk::crypto::mlkem


#endif // #ifndef SDK_CRYPTO_MLKEM_POLY_H