#ifndef CRYPTO_SHA_SHA3_H
#define CRYPTO_SHA_SHA3_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

#include "sdk/crypto/macro.h"


namespace crypto::sha
{


struct keccak_ctx 
{
  uint64_t s_[25];
  unsigned int p_;
  unsigned int r_;
  uint8_t suffix_;
};


inline void make_keccak_ctx(shake_set set, keccak_ctx *ctx) noexcept {
  ctx->p_ = 0;
  ctx->suffix_ = shake_suffix;
  switch (set) {
    case shake_set::shake128: ctx->r_ = shake128_rate; break;
    case shake_set::shake256: ctx->r_ = shake256_rate; break;
    default: return;
  }
}


void shake_init(keccak_ctx *ctx);

void shake_absorb(keccak_ctx *ctx, const uint8_t *in, size_t inlen);

void shake_finalize(keccak_ctx *ctx);

void shake_squeeze(keccak_ctx *ctx, uint8_t *out, size_t len);

void shake_absorb_once(keccak_ctx *ctx, const uint8_t *in, size_t inlen);

void shake_squeeze_blocks(keccak_ctx *ctx, uint8_t *out, size_t nblocks);

/**
 * Compute the requested number of SHAKE output bytes.
 * @param out destination buffer with space for the requested output.
 * @param outlen number of output bytes.
 * @param in input buffer containing at least inlen readable bytes.
 * @param inlen number of input bytes.
 */
void shake(shake_set set, uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen) noexcept;


/**
 * Compute a SHA3 digest.
 * @param out destination buffer with space for the requested output.
 * @param in input buffer containing at least inlen readable bytes.
 * @param inlen number of input bytes.
 */
void sha3(sha3_set set, uint8_t *out, const uint8_t *in, size_t inlen) noexcept;


} // namespace crypto::sha


#endif // #ifndef CRYPTO_SHA_SHA3_H