#include "sdk/crypto/mlkem/mlkem.h"

#include <cstddef>
#include <cstdint>
#include <cstring>


namespace sdk::crypto::mlkem
{


  namespace impl
  {


  inline uint32_t load32_le(const uint8_t bytes[4]) {
    return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8 | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
  }

  inline uint32_t load24_le(const uint8_t bytes[3]) {
    return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8 | (uint32_t)bytes[2] << 16;
  }


  /**
   * Central binomial distribution (eta = 2) to generate small noise poly
   * for eta = 2,
   * for each coefficient, we need 4 bits as
   * coeff = sum_{i=1}^eta(a_i) - sum_{i=(eta+1)}^{2*eta}(a_i)
   * @param rand_bytes len: 4*mlkem_n/8
   */
  inline void cbd2(poly *p, const uint8_t rand_bytes[4*mlkem_n/8]) {
    uint32_t t, d;
    int16_t a, b;
    for (int i = 0; i < mlkem_n / 8; ++i) {
      t = load32_le(rand_bytes+4*i);
      d = t & 0x55555555;
      d += (t >> 1) & 0x55555555;
      for (int j = 0; j < 8; ++j) {
        a = (d >> (4 * j + 0)) & 0x3;
        b = (d >> (4 * j + 2)) & 0x3;
        p->coeffs[8*i+j] = a - b;
      }
    }
  }

  inline void cbd3(poly *p, const uint8_t rand_bytes[6*mlkem_n/8]) {
    uint32_t t, d;
    int16_t a, b;
    for (int i = 0; i < mlkem_n / 4; ++i) {
      t = load24_le(rand_bytes+3*i);
      d = t & 0x00249249;
      d += (t >> 1) & 0x00249249;
      d += (t >> 2) & 0x00249249;
      for (int j = 0; j < 4; ++j) {
        a = (d >> (6 * j + 0)) & 0x7;
        b = (d >> (6 * j + 3)) & 0x7;
        p->coeffs[4*i+j] = a - b;
      }
    }
  }

  /**
   * Central binomial distribution to generate small noise polynomial e
   * @param rand_bytes len: mlkem_eta1*mlkem_n/4
   */
  inline void cbd_eta1(const mlkem_ctx *ctx, poly *p, const uint8_t *rand_bytes) {
    switch (ctx->eta1_) {
      case 2:
        cbd2(p, rand_bytes);
        break;
      case 3:
        cbd3(p, rand_bytes);
        break;
      default:
        return;
    }
  }

  /**
   * Central binomial distribution to generate small ephemeral noise polynomial
   */
  inline void cbd_eta2(poly *p, const uint8_t rand_bytes[mlkem_eta2*mlkem_n/4]) {
    cbd2(p, rand_bytes);
  }

  inline constexpr size_t mat_nblocks =
    (12 * mlkem_n / 8 * (1 << 12) / mlkem_q + sha::shake128_rate) / sha::shake128_rate;


  static inline unsigned int rej_sample_uniform(
    int16_t *ptr, unsigned int len, const uint8_t *buf, unsigned int buflen
  ) {
    unsigned int cnt = 0, pos = 0;
    uint16_t v0, v1;
    while (cnt < len && pos + 3 <= buflen) {
      v0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xfff;
      v1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xfff;
      pos += 3;
      if (v0 < mlkem_q) {
        ptr[cnt++] = v0;
      }
      if (cnt < len && v1 < mlkem_q) {
        ptr[cnt++] = v1;
      }
    }
    return cnt;
  }

  static inline void gen_matrix(const mlkem_ctx *ctx, poly_vec *a, const uint8_t seed[seed_len], int transposed) {
    unsigned int cnt, buflen;
    uint8_t buf[mat_nblocks*sha::shake128_rate];
    sha::keccak_ctx keccak_ctx;
    for (int i = 0; i < ctx->k_; ++i) {
      for (int j = 0; j < ctx->k_; ++j) {
        if (transposed) {
          shake128_absorb(&keccak_ctx, seed, i, j);
        } else {
          shake128_absorb(&keccak_ctx, seed, j, i);
        }

        sha::shake_squeeze_blocks(&keccak_ctx, buf, mat_nblocks);
        buflen = mat_nblocks * sha::shake128_rate;
        cnt = rej_sample_uniform(a[i].vec[j].coeffs, mlkem_n, buf, buflen);

        while (cnt < mlkem_n) {
          sha::shake_squeeze_blocks(&keccak_ctx, buf, 1);
          buflen = sha::shake128_rate;
          cnt += rej_sample_uniform(a[i].vec[j].coeffs+cnt, mlkem_n-cnt, buf, buflen);
        }
      }
    }
  }


  inline void gen_noise_poly_eta1(const mlkem_ctx *ctx, poly *p, const uint8_t seed[seed_len], uint8_t nonce) {
    uint8_t buf[mlkem_max_eta1*mlkem_n/4];
    shake256_prf(buf, ctx->eta1_*mlkem_n/4, seed, nonce);
    cbd_eta1(ctx, p, buf);
  }

  inline void gen_noise_poly_eta2(poly *p, const uint8_t seed[seed_len], uint8_t nonce) {
    uint8_t buf[mlkem_eta2*mlkem_n/4];
    shake256_prf(buf, sizeof(buf), seed, nonce);
    cbd_eta2(p, buf);
  }


  /**
   * @param ek len: poly_vec_len + seed_len
   */
  inline void pack_ek(const mlkem_ctx *ctx, uint8_t *ek, poly_vec *ekpv, const uint8_t seed[seed_len]) {
    poly_vec_to_bytes(ctx, ekpv, ek);
    memcpy(ek+ctx->poly_vec_len_, seed, seed_len);
  }

  /**
   * @param ek len: poly_vec_len + seed_len
   */
  inline void unpack_ek(const mlkem_ctx *ctx, poly_vec *ekpv, uint8_t seed[seed_len], const uint8_t *ek) {
    bytes_to_poly_vec(ctx, ek, ekpv);
    memcpy(seed, ek+ctx->poly_vec_len_, seed_len);
  }

  /**
   * @param dk len: poly_vec_len
   */
  inline void pack_dk(const mlkem_ctx *ctx, uint8_t *dk, const poly_vec *dkpv) {
    poly_vec_to_bytes(ctx, dkpv, dk);
  }

  static inline void unpack_dk(const mlkem_ctx *ctx, poly_vec *dkpv, const uint8_t *dk) {
    bytes_to_poly_vec(ctx, dk, dkpv);
  }

  /**
   * A cipher is comprised of a poly vec u:
   *   u = A^Ty + e_1
   * and a poly v:
   *   v = t^Ty + e_2 + mu
   * @param c len : compressed_poly_vec_len + compressed_poly_len
   */
  inline void pack_cipher(const mlkem_ctx *ctx, uint8_t *c, const poly_vec *u, const poly *v) {
    poly_vec_compress(ctx, u, c);
    poly_compress(ctx, v, c+ctx->compressed_poly_vec_len_);
  }

  inline void unpack_cipher(const mlkem_ctx *ctx, poly_vec *u, poly *v, const uint8_t *c) {
    poly_vec_decompress(ctx, c, u);
    poly_decompress(ctx, c+ctx->compressed_poly_vec_len_, v);
  }


  /**
   * @param ek len: poly_vec_len + seed_len
   * @param dk len: poly_vec_len
   */
  inline void indcpa_key_gen(const mlkem_ctx *ctx, uint8_t *ek, uint8_t *dk, const uint8_t seeds[seed_len]) {
    uint8_t buf[2*seed_len]; /// used to store seeds
    const uint8_t *pub_seed = buf;
    const uint8_t *noise_seed = buf + seed_len;

    memcpy(buf, seeds, seed_len);
    buf[seed_len] = ctx->k_;
    /// expand 32+1 bytes to two pseudorandom 32-byte seeds
    crypto::sha::sha3(crypto::sha::sha3_set::sha3_512, buf, buf, seed_len+1);

    uint8_t nonce = 0;
    poly_vec a[mlkem_max_k], e, ekpv, dkpv, b;

    gen_matrix(ctx, a, pub_seed, 0);
    for (int i = 0; i < ctx->k_; ++i) {
      gen_noise_poly_eta1(ctx, &dkpv.vec[i], noise_seed, nonce++);
    }
    for (int i = 0; i < ctx->k_; ++i) {
      gen_noise_poly_eta1(ctx, &e.vec[i], noise_seed, nonce++);
    }

    poly_vec_ntt(ctx, &dkpv);
    poly_vec_ntt(ctx, &e);
    for (int i = 0; i < ctx->k_; ++i) {
      poly_vec_basemul(ctx, &ekpv.vec[i], &a[i], &dkpv);
      poly_tomont(&ekpv.vec[i]);
    }
    poly_vec_add(ctx, &ekpv, &ekpv, &e);
    poly_vec_reduce(ctx, &ekpv);

    pack_ek(ctx, ek, &ekpv, pub_seed);
    pack_dk(ctx, dk, &dkpv);
  }

  /**
   * @param c len: compressed_poly_vec_len+compressed_poly_len
   * @param m len: mlkem_msg_len
   * @param ek len: mlkem_poly_vec_len + seed_len
   */
  inline void indcpa_enc(const mlkem_ctx *ctx, uint8_t *c, const uint8_t m[mlkem_msg_len], const uint8_t *ek,
                        const uint8_t seed[seed_len]) {
    uint8_t seed_received[seed_len];
    uint8_t nonce = 0;

    poly_vec at[mlkem_max_k], y, u, e1, ekpv;
    poly v, e2, mu;

    impl::unpack_ek(ctx, &ekpv, seed_received, ek);
    msg_to_poly(&mu, m); /// convert msg to poly form

    /// the encap process of calculating
    /// u = A^{T}y + e1, v = t^{T}y + e2 + u */
    gen_matrix(ctx, at, seed_received, 1); /// regen mat a used in key gen
    for (int i = 0; i < ctx->k_; ++i) {
      gen_noise_poly_eta1(ctx, &y.vec[i], seed, nonce++);
    }
    for (int i = 0; i < ctx->k_; ++i) {
      gen_noise_poly_eta2(&e1.vec[i], seed, nonce++);
    }
    gen_noise_poly_eta2(&e2, seed, nonce++);

    poly_vec_ntt(ctx, &y);
    for (int i = 0; i < ctx->k_; ++i) {
      poly_vec_basemul(ctx, &u.vec[i], &at[i], &y); /// u = A^{T}y + e1 */
    }
    poly_vec_basemul(ctx, &v, &ekpv, &y); /// v = t^{T}y + e2 + mu */

    poly_vec_invntt(ctx, &u);
    poly_invntt(&v);

    poly_vec_add(ctx, &u, &u, &e1);
    poly_add(&v, &v, &e2);
    poly_add(&v, &v, &mu);
    poly_vec_reduce(ctx, &u);
    poly_reduce(&v);

    pack_cipher(ctx, c, &u, &v); /// byte encode
  }

  /**
   * @param m len: mlkem_msg_len
   * @param cipher len: compressed_poly_vec_len+compressed_poly_len
   * @param dk len: mlkem_poly_vec_len
   */
  inline void indcpa_dec(const mlkem_ctx *ctx, uint8_t m[mlkem_msg_len], const uint8_t *cipher, uint8_t *dk) {
    poly_vec u, dkpv;
    poly v, w;

    unpack_cipher(ctx, &u, &v, cipher);
    unpack_dk(ctx, &dkpv, dk); /// dkpv is now in the ntt domain

    /// process of calculating
    /// w = v' - invntt(s^T(u'))
    /// notice the u and v will be different from u and v in encap as compression
    /// and decompression alter their values
    poly_vec_ntt(ctx, &u); // u to ntt domain

    poly_vec_basemul(ctx, &w, &dkpv, &u);
    poly_invntt(&w);
    poly_sub(&w, &v, &w);
    poly_reduce(&w);

    poly_to_msg(m, &w);
  }


  } // namespace impl

  
void mlkem_key_gen(mlkem_set set, uint8_t *ek, uint8_t *dk) noexcept {
  mlkem_ctx ctx;
  make_mlkem_ctx(set, &ctx);

  uint8_t seeds[2*seed_len];
  gen_rand_bytes(seeds, 2*seed_len);
  impl::indcpa_key_gen(&ctx, ek, dk, seeds);

  /// store ek(ek and pub seed to gen matrix) after dk
  memcpy(dk+ctx.poly_vec_len_, ek, ctx.poly_vec_len_+seed_len);
  /// also store ek hash
  sha::sha3(sha::sha3_set::sha3_256, dk+ctx.poly_vec_len_+ctx.poly_vec_len_+seed_len, ek,
            ctx.poly_vec_len_+seed_len);
  /// rejection value z
  memcpy(dk+2*ctx.poly_vec_len_+seed_len+sha::hash256_len, seeds+seed_len, seed_len);
}

void mlkem_encap(mlkem_set set, uint8_t *msg, uint8_t *sk, const uint8_t *ek) noexcept {
  mlkem_ctx ctx;
  make_mlkem_ctx(set, &ctx);

  uint8_t seed[seed_len];
  gen_rand_bytes(seed, seed_len);

  uint8_t buf[seed_len+sha::hash256_len];
  uint8_t kr[64]; /// hash of key and randomness

  memcpy(buf, seed, seed_len);

  sha::sha3(sha::sha3_set::sha3_256, buf+seed_len, ek, ctx.poly_vec_len_+seed_len);
  sha::sha3(sha::sha3_set::sha3_512, kr, buf, seed_len+sha::hash256_len);

  impl::indcpa_enc(&ctx, msg, buf, ek, kr+seed_len);

  memcpy(sk, kr, seed_len);
}

void mlkem_decap(mlkem_set set, uint8_t *ss, uint8_t *cipher, uint8_t *dk) noexcept {
  mlkem_ctx ctx;
  make_mlkem_ctx(set, &ctx);

  int fail;
  /// cipher text decrypted, contains shared key and random seed for fo transform
  uint8_t buf[mlkem_msg_len+sha::hash256_len];
  /// key and random seed used to re-encrypt
  uint8_t kr[mlkem_msg_len+seed_len];
  /// used to store newly generated cipher
  uint8_t cmp[mlkem_max_compressed_poly_vec_len+mlkem_max_compressed_poly_len];
  const uint8_t *ek = dk + ctx.poly_vec_len_;

  impl::indcpa_dec(&ctx, buf, cipher, dk);

  memcpy(buf+mlkem_msg_len, dk+2*ctx.poly_vec_len_+seed_len, sha::hash256_len); /// extract hash of encap key
  sha::sha3(sha::sha3_set::sha3_512, kr, buf, mlkem_msg_len+sha::hash256_len);

  impl::indcpa_enc(&ctx, cmp, buf, ek, kr+mlkem_msg_len); /// recalculate encrypted msg

  fail = ccmp(cipher, cmp, ctx.compressed_poly_vec_len_+ctx.compressed_poly_len_);

  shake256_rkprf(&ctx, ss, dk+2*ctx.poly_vec_len_+seed_len+sha::hash256_len, cipher); /// compute rejection key

  cmov_uint8(kr, ss, seed_len, !fail);
}


} // namespace sdk::crypto::mlkem