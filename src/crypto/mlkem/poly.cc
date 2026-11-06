#include "sdk/crypto/mlkem/poly.h"

#include "sdk/crypto/mlkem/mlkem.h"


namespace sdk::crypto::mlkem
{


void poly_to_bytes(uint8_t out[mlkem_poly_len], const poly *p) {
  uint16_t t0, t1;
  for (int i = 0; i < mlkem_n / 2; ++i) {
    t0 = p->coeffs[2*i];
    t1 = p->coeffs[2*i+1];
    t0 += ((int16_t)t0 >> 15) & mlkem_q;
    t1 += ((int16_t)t1 >> 15) & mlkem_q;
    out[3*i+0] = (t0 >> 0);
    out[3*i+1] = (t0 >> 8 | t1 << 4);
    out[3*i+2] = (t1 >> 4);
  }
}

void bytes_to_poly(poly *p, const uint8_t in[mlkem_poly_len]) {
  for (int i = 0; i < mlkem_n / 2; ++i) {
    p->coeffs[2*i+0] = ((in[3*i+0] >> 0) | (uint16_t)in[3*i+1] << 8) & 0x0fff;
    p->coeffs[2*i+1] = ((in[3*i+1] >> 4) | (uint16_t)in[3*i+2] << 4) & 0x0fff;
  }
}

void poly_reduce(poly *p) {
  for (int i = 0; i < mlkem_n; ++i) {
    p->coeffs[i] = barrett_reduce(p->coeffs[i]);
  }
}

void poly_tomont(poly *p) {
  const int16_t f = (1ULL << 32) % mlkem_q;
  for (int i = 0; i < mlkem_n; ++i) {
    p->coeffs[i] = montgomery_reduce((int32_t)p->coeffs[i]*f);
  }
}

void poly_ntt(poly *p) {
  ntt(p->coeffs);
  poly_reduce(p);
}

void poly_invntt(poly *p) {
  inv_ntt(p->coeffs);
}

void poly_add(poly *p, const poly *a, const poly *b) {
  for (int i = 0; i < mlkem_n; ++i) {
    p->coeffs[i] = a->coeffs[i] + b->coeffs[i];
  }
}

void poly_sub(poly *p, const poly *a, const poly *b) {
  for (int i = 0; i < mlkem_n; ++i) {
    p->coeffs[i] = a->coeffs[i] - b->coeffs[i];
  }
}

void poly_basemul(poly *p, const poly *a, const poly *b) {
  for (int i = 0; i < mlkem_n / 4; ++i) {
    basemul(&p->coeffs[4*i], &a->coeffs[4*i], &b->coeffs[4*i], zetas[64+i]);
    basemul(&p->coeffs[4*i+2], &a->coeffs[4*i+2], &b->coeffs[4*i+2], -zetas[64+i]);
  }
}

void poly_compress(const mlkem_ctx *ctx, const poly *p, uint8_t *out) {
  int16_t u;
  uint32_t d0;
  uint8_t t[8];
  if (ctx->compressed_poly_len_ == 128) {
    for (int i = 0; i < mlkem_n / 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        u = p->coeffs[8*i+j];
        u += (u >> 15) & mlkem_q;
        d0 = u << 4;
        d0 += 1665;
        d0 *= 80635;
        d0 >>= 28;
        t[j] = d0 & 0xf;
      }
      out[0] = t[0] | (t[1] << 4);
      out[1] = t[2] | (t[3] << 4);
      out[2] = t[4] | (t[5] << 4);
      out[3] = t[6] | (t[7] << 4);
      out += 4;
    }
  } else if (ctx->compressed_poly_len_ == 160) {
    for (int i = 0; i < mlkem_n / 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        u = p->coeffs[8*i+j];
        u += (u >> 15) & mlkem_q;
        d0 = u << 5;
        d0 += 1664;
        d0 *= 40318;
        d0 >>= 27;
        t[j] = d0 & 0x1f;
      }
      out[0] = (t[0] >> 0) | (t[1] << 5);
      out[1] = (t[1] >> 3) | (t[2] << 2) | (t[3] << 7);
      out[2] = (t[3] >> 1) | (t[4] << 4);
      out[3] = (t[4] >> 4) | (t[5] << 1) | (t[6] << 6);
      out[4] = (t[6] >> 2) | (t[7] << 3);
      out += 5;
    }
  } else {
    // todo
  }
}

void poly_decompress(const mlkem_ctx *ctx, const uint8_t *in, poly *p) {
  if (ctx->compressed_poly_len_ == 128) {
    for (int i = 0; i < mlkem_n / 2; ++i) {
      p->coeffs[2*i+0] = (((uint16_t)(in[0] & 15) * mlkem_q) + 8) >> 4;
      p->coeffs[2*i+1] = (((uint16_t)(in[0] >> 4) * mlkem_q) + 8) >> 4;
      in += 1;
    }
  } else if (ctx->compressed_poly_len_ == 160) {
    unsigned int j;
    uint8_t t[8];
    for (int i = 0; i < mlkem_n / 8; ++i) {
      t[0] = (in[0] >> 0);
      t[1] = (in[0] >> 5) | (in[1] << 3);
      t[2] = (in[1] >> 2);
      t[3] = (in[1] >> 7) | (in[2] << 1);
      t[4] = (in[2] >> 4) | (in[3] << 4);
      t[5] = (in[3] >> 1);
      t[6] = (in[3] >> 6) | (in[4] << 2);
      t[7] = (in[4] >> 3);
      in += 5;
      for (int j = 0; j < 8; ++j) {
        p->coeffs[8*i+j] = ((uint32_t)(t[j] & 31) * mlkem_q + 16) >> 5;
      }
    }
  } else {
    // todo
  }
}

void poly_to_msg(uint8_t m[mlkem_msg_len], const poly *p) {
  uint32_t t;
  for (int i = 0; i < mlkem_n / 8; ++i) {
    m[i] = 0;
    for (int j = 0; j < 8; ++j) {
      t = p->coeffs[8*i+j];
      t <<= 1;
      t += 1665;
      t *= 80635;
      t >>= 28;
      t &= 1;
      m[i] |= t << j;
    }
  }
}

void msg_to_poly(poly *p, const uint8_t m[mlkem_msg_len]) {
  for (int i = 0; i < mlkem_n / 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      p->coeffs[8*i+j] = 0;
      cmov_int16(((mlkem_q+1)/2), p->coeffs+8*i+j, ((m[i]>>j)&1));
    }
  }
}


void poly_vec_to_bytes(const mlkem_ctx *ctx, const poly_vec *pv, uint8_t *out) {
  for (int i = 0; i < ctx->k_; ++i) {
    poly_to_bytes(out+i*mlkem_poly_len, &pv->vec[i]);
  }
}

void bytes_to_poly_vec(const mlkem_ctx *ctx, const uint8_t *in, poly_vec *pv) {
  for (int i = 0; i < ctx->k_; ++i) {
    bytes_to_poly(&pv->vec[i], in+i*mlkem_poly_len);
  }
}

void poly_vec_reduce(const mlkem_ctx *ctx, poly_vec *pv) {
  for (int i = 0; i < ctx->k_; ++i) {
    poly_reduce(&pv->vec[i]);
  }
}

void poly_vec_ntt(const mlkem_ctx *ctx, poly_vec *pv) {
  for (int i = 0; i < ctx->k_; ++i) {
    poly_ntt(&pv->vec[i]);
  }
}

void poly_vec_invntt(const mlkem_ctx *ctx, poly_vec *pv) {
  for (int i = 0; i < ctx->k_; ++i) {
    poly_invntt(&pv->vec[i]);
  }
}

void poly_vec_add(const mlkem_ctx *ctx, poly_vec *pv, const poly_vec *a, const poly_vec *b) {
  for (int i = 0; i < ctx->k_; ++i) {
    poly_add(&pv->vec[i], &a->vec[i], &b->vec[i]);
  }
}

void poly_vec_basemul(const mlkem_ctx *ctx, poly *p, const poly_vec *a, const poly_vec *b) {
  poly t;
  poly_basemul(p, &a->vec[0], &b->vec[0]);
  for (int i = 1; i < ctx->k_; ++i) {
    poly_basemul(&t, &a->vec[i], &b->vec[i]);
    poly_add(p, p, &t);
  }
  poly_reduce(p);
}

void poly_vec_compress(const mlkem_ctx *ctx, const poly_vec *p, uint8_t *out) {
  uint64_t d0;
  if (ctx->compressed_poly_vec_len_ == ctx->k_ * 352) {
    uint16_t t[8];
    for (int i = 0; i < ctx->k_; ++i) {
      for (int j = 0; j < mlkem_n / 8; ++j) {
        for (int m = 0; m < 8; ++m) {
          t[m] = p->vec[i].coeffs[8*j+m];
          t[m] += ((int16_t)t[m] >> 15) & mlkem_q;
          d0 = t[m];
          d0 <<= 11;
          d0 += 1664;
          d0 *= 645084;
          d0 >>= 31;
          t[m] = d0 & 0x7ff;
        }
        out[0] = (t[0] >> 0);
        out[1] = (t[0] >> 8) | (t[1] << 3);
        out[2] = (t[1] >> 5) | (t[2] << 6);
        out[3] = (t[2] >> 2);
        out[4] = (t[2] >> 10) | (t[3] << 1);
        out[5] = (t[3] >> 7) | (t[4] << 4);
        out[6] = (t[4] >> 4) | (t[5] << 7);
        out[7] = (t[5] >> 1);
        out[8] = (t[5] >> 9) | (t[6] << 2);
        out[9] = (t[6] >> 6) | (t[7] << 5);
        out[10] = (t[7] >> 3);
        out += 11;
      }
    }
  } else if (ctx->compressed_poly_vec_len_ == ctx->k_ * 320) {
    uint16_t t[4];
    for (int i = 0; i < ctx->k_; ++i) {
      for (int j = 0; j < mlkem_n / 4; ++j) {
        for (int m = 0; m < 4; ++m) {
          t[m] = p->vec[i].coeffs[4*j+m];
          t[m] += ((int16_t)t[m] >> 15) & mlkem_q;
          d0 = t[m];
          d0 <<= 10;
          d0 += 1665;
          d0 *= 1290167;
          d0 >>= 32;
          t[m] = d0 & 0x3ff;
        }
        out[0] = (t[0] >> 0);
        out[1] = (t[0] >> 8) | (t[1] << 2);
        out[2] = (t[1] >> 6) | (t[2] << 4);
        out[3] = (t[2] >> 4) | (t[3] << 6);
        out[4] = (t[3] >> 2);
        out += 5;
      }
    }
  }
}

void poly_vec_decompress(const mlkem_ctx *ctx, const uint8_t *in, poly_vec *p) {
  if (ctx->compressed_poly_vec_len_ == ctx->k_ * 352) {
    uint16_t t[8];
    for (int i = 0; i < ctx->k_; ++i) {
      for (int j = 0; j < mlkem_n / 8; ++j) {
        t[0] = (in[0] >> 0) | ((uint16_t)in[1] << 8);
        t[1] = (in[1] >> 3) | ((uint16_t)in[2] << 5);
        t[2] = (in[2] >> 6) | ((uint16_t)in[3] << 2) | ((uint16_t)in[4] << 10);
        t[3] = (in[4] >> 1) | ((uint16_t)in[5] << 7);
        t[4] = (in[5] >> 4) | ((uint16_t)in[6] << 4);
        t[5] = (in[6] >> 7) | ((uint16_t)in[7] << 1) | ((uint16_t)in[8] << 9);
        t[6] = (in[8] >> 2) | ((uint16_t)in[9] << 6);
        t[7] = (in[9] >> 5) | ((uint16_t)in[10] << 3);
        in += 11;
        for (int m = 0; m < 8; ++m) {
          p->vec[i].coeffs[8*j+m] = ((uint32_t)(t[m] & 0x7ff) * mlkem_q + 1024) >> 11;
        }
      }
    }
  } else if (ctx->compressed_poly_vec_len_ == ctx->k_ * 320) {
    uint16_t t[4];
    for (int i = 0; i < ctx->k_; ++i) {
      for (int j = 0; j < mlkem_n / 4; ++j) {
        t[0] = (in[0] >> 0) | ((uint16_t)in[1] << 8);
        t[1] = (in[1] >> 2) | ((uint16_t)in[2] << 6);
        t[2] = (in[2] >> 4) | ((uint16_t)in[3] << 4);
        t[3] = (in[3] >> 6) | ((uint16_t)in[4] << 2);
        in += 5;
        for (int m = 0; m < 4; ++m) {
          p->vec[i].coeffs[4*j+m] = ((uint32_t)(t[m] & 0x3ff) * mlkem_q + 512) >> 10;
        }
      }
    }
  }
}


} // namespace sdk::crypto::mlkem