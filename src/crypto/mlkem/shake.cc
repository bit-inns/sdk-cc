#include "sdk/crypto/mlkem/shake.h"

#include <cstring>

#include "sdk/crypto/mlkem/mlkem.h"


namespace sdk::crypto::mlkem
{


void shake128_absorb(sha::keccak_ctx *ctx, const uint8_t seed[seed_len], uint8_t x, uint8_t y) {
  sha::make_keccak_ctx(sha::shake_set::shake128, ctx);
  uint8_t extended_seed[seed_len+2];
  memcpy(extended_seed, seed, seed_len);
  extended_seed[seed_len+0] = x;
  extended_seed[seed_len+1] = y;
  sha::shake_absorb_once(ctx, extended_seed, sizeof(extended_seed));
}

void shake256_prf(uint8_t *out, size_t len, const uint8_t key[seed_len], uint8_t nonce) {
  uint8_t buf[seed_len+1];
  memcpy(buf, key, seed_len);
  buf[seed_len] = nonce;
  sha::shake(crypto::sha::shake_set::shake256, out, len, buf, sizeof(buf));
}

void shake256_rkprf(
  const mlkem_ctx *mlkem_ctx, uint8_t out[sha::hash256_len], const uint8_t key[seed_len], const uint8_t *input
) {
  sha::keccak_ctx keccak_ctx;
  keccak_ctx.p_ = 0;
  keccak_ctx.r_ = sha::shake256_rate;
  keccak_ctx.suffix_ = sha::shake_suffix;
  sha::shake_init(&keccak_ctx);
  sha::shake_absorb(&keccak_ctx, key, seed_len);
  sha::shake_absorb(&keccak_ctx, input, mlkem_ctx->compressed_poly_vec_len_+mlkem_ctx->compressed_poly_len_);
  sha::shake_finalize(&keccak_ctx);
  sha::shake_squeeze(&keccak_ctx, out, mlkem_msg_len);
}


} // namespace sdk::crypto::mlkem