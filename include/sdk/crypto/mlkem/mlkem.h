#ifndef CRYPTO_MLKEM_MLKEM_H
#define CRYPTO_MLKEM_MLKEM_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "sdk/crypto/ctops.h"
#include "sdk/crypto/macro.h"
#include "sdk/crypto/mlkem/ntt.h"
#include "sdk/crypto/mlkem/poly.h"
#include "sdk/crypto/mlkem/shake.h"
#include "sdk/crypto/rand.h"
#include "sdk/crypto/sha/sha3.h"


namespace crypto::mlkem
{


struct mlkem_ctx
{
  unsigned int k_;
  unsigned int eta1_;
  unsigned int compressed_poly_len_;
  unsigned int poly_vec_len_;
  unsigned int compressed_poly_vec_len_;
  unsigned int ek_len_;
  unsigned int dk_len_;
};


inline void make_mlkem_ctx(mlkem_set set, mlkem_ctx *ctx) noexcept {
  switch (set) {
    case mlkem_set::mlkem_512:
      ctx->k_ = mlkem512_k;
      ctx->eta1_ = mlkem512_eta1;
      ctx->compressed_poly_len_ = mlkem512_compressed_poly_len;
      ctx->poly_vec_len_ = mlkem512_poly_vec_len;
      ctx->compressed_poly_vec_len_ = mlkem512_compressed_poly_vec_len;
      ctx->ek_len_ = mlkem512_ek_len;
      ctx->dk_len_ = mlkem512_dk_len;
      break;
    case mlkem_set::mlkem_768:
      ctx->k_ = mlkem768_k;
      ctx->eta1_ = mlkem768_eta1;
      ctx->compressed_poly_len_ = mlkem768_compressed_poly_len;
      ctx->poly_vec_len_ = mlkem768_poly_vec_len;
      ctx->compressed_poly_vec_len_ = mlkem768_compressed_poly_vec_len;
      ctx->ek_len_ = mlkem768_ek_len;
      ctx->dk_len_ = mlkem768_dk_len;
      break;
    case mlkem_set::mlkem_1024:
      ctx->k_ = mlkem1024_k;
      ctx->eta1_ = mlkem1024_eta1;
      ctx->compressed_poly_len_ = mlkem1024_compressed_poly_len;
      ctx->poly_vec_len_ = mlkem1024_poly_vec_len;
      ctx->compressed_poly_vec_len_ = mlkem1024_compressed_poly_vec_len;
      ctx->ek_len_ = mlkem1024_ek_len;
      ctx->dk_len_ = mlkem1024_dk_len;
      break;
    default:
      return;
  }
}

/**
 * Key generation scheme, specified as algo 16 and 19
 * @param ek: output encapsulation key
 * @param dk: output decapsulation key */
void mlkem_key_gen(mlkem_set set, uint8_t *ek, uint8_t *dk) noexcept;

/**
 * Encapsulate, specified as algo 17 and 20
 * @param cipher: cipher text
 * @param sk: the shared secret key used for future symmetric encryption
 * @param ek: the encryption key received */
void mlkem_encap(mlkem_set set, uint8_t *cipher, uint8_t *sk, const uint8_t *ek) noexcept;

/**
 * Decapsulation, specified as algo 18 and 21
 * @param ss: output shared secret key
 */
void mlkem_decap(mlkem_set set, uint8_t *ss, uint8_t *cipher, uint8_t *dk) noexcept;


} // namespace crypto::mlkem


#endif // #ifndef CRYPTO_MLKEM_MLKEM_H