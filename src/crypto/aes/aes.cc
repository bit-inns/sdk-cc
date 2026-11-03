#include "sdk/crypto/aes/aes.h"


namespace crypto::aes
{


  namespace impl
  {


  void xtime_word(uint32_t *w) {
    uint32_t a, b;
    a = *w;
    b = a & 0x80808080u;
    a ^= b;
    b -= b >> 7;
    b &= 0x1B1B1B1Bu;
    b ^= a << 1;
    *w = b;
  }
  void xtime_long(uint64_t *w) {
    uint64_t a, b;
    a = *w;
    b = a & 0x8080808080808080ULL;
    a ^= b;
    b -= b >> 7;
    b &= 0x1B1B1B1B1B1B1B1BULL;
    b ^= a << 1;
    *w = b;
  }


  void sub_word(uint32_t *w) {
    uint32_t x, y, a1, a2, a3, a4, a5, a6;
    x = *w;
    y = ((x & 0xFEFEFEFEu) >> 1) | ((x & 0x01010101u) << 7);
    x &= 0xDDDDDDDDu;
    x ^= y & 0x57575757u;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x1C1C1C1Cu;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x4A4A4A4Au;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x42424242u;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x64646464u;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0xE0E0E0E0u;
    a1 = x;
    a1 ^= (x & 0xF0F0F0F0u) >> 4;
    a2 = ((x & 0xCCCCCCCCu) >> 2) | ((x & 0x33333333u) << 2);
    a3 = x & a1;
    a3 ^= (a3 & 0xAAAAAAAAu) >> 1;
    a3 ^= (((x << 1) & a1) ^ ((a1 << 1) & x)) & 0xAAAAAAAAu;
    a4 = a2 & a1;
    a4 ^= (a4 & 0xAAAAAAAAu) >> 1;
    a4 ^= (((a2 << 1) & a1) ^ ((a1 << 1) & a2)) & 0xAAAAAAAAu;
    a5 = (a3 & 0xCCCCCCCCu) >> 2;
    a3 ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCu;
    a4 = a5 & 0x22222222u;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x22222222u;
    a3 ^= a4;
    a5 = a3 & 0xA0A0A0A0u;
    a5 |= a5 >> 1;
    a5 ^= (a3 << 1) & 0xA0A0A0A0u;
    a4 = a5 & 0xC0C0C0C0u;
    a6 = a4 >> 2;
    a4 ^= (a5 << 2) & 0xC0C0C0C0u;
    a5 = a6 & 0x20202020u;
    a5 |= a5 >> 1;
    a5 ^= (a6 << 1) & 0x20202020u;
    a4 |= a5;
    a3 ^= a4 >> 4;
    a3 &= 0x0F0F0F0Fu;
    a2 = a3;
    a2 ^= (a3 & 0x0C0C0C0Cu) >> 2;
    a4 = a3 & a2;
    a4 ^= (a4 & 0x0A0A0A0Au) >> 1;
    a4 ^= (((a3 << 1) & a2) ^ ((a2 << 1) & a3)) & 0x0A0A0A0Au;
    a5 = a4 & 0x08080808u;
    a5 |= a5 >> 1;
    a5 ^= (a4 << 1) & 0x08080808u;
    a4 ^= a5 >> 2;
    a4 &= 0x03030303u;
    a4 ^= (a4 & 0x02020202u) >> 1;
    a4 |= a4 << 2;
    a3 = a2 & a4;
    a3 ^= (a3 & 0x0A0A0A0Au) >> 1;
    a3 ^= (((a2 << 1) & a4) ^ ((a4 << 1) & a2)) & 0x0A0A0A0Au;
    a3 |= a3 << 4;
    a2 = ((a1 & 0xCCCCCCCCu) >> 2) | ((a1 & 0x33333333u) << 2);
    x = a1 & a3;
    x ^= (x & 0xAAAAAAAAu) >> 1;
    x ^= (((a1 << 1) & a3) ^ ((a3 << 1) & a1)) & 0xAAAAAAAAu;
    a4 = a2 & a3;
    a4 ^= (a4 & 0xAAAAAAAAu) >> 1;
    a4 ^= (((a2 << 1) & a3) ^ ((a3 << 1) & a2)) & 0xAAAAAAAAu;
    a5 = (x & 0xCCCCCCCCu) >> 2;
    x ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCu;
    a4 = a5 & 0x22222222u;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x22222222u;
    x ^= a4;
    y = ((x & 0xFEFEFEFEu) >> 1) | ((x & 0x01010101u) << 7);
    x &= 0x39393939u;
    x ^= y & 0x3F3F3F3Fu;
    y = ((y & 0xFCFCFCFCu) >> 2) | ((y & 0x03030303u) << 6);
    x ^= y & 0x97979797u;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x9B9B9B9Bu;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x3C3C3C3Cu;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0xDDDDDDDDu;
    y = ((y & 0xFEFEFEFEu) >> 1) | ((y & 0x01010101u) << 7);
    x ^= y & 0x72727272u;
    x ^= 0x63636363u;
    *w = x;
  }
  void sub_long(uint64_t *w) {
    uint64_t x, y, a1, a2, a3, a4, a5, a6;
    x = *w;
    y = ((x & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((x & 0x0101010101010101ULL) << 7);
    x &= 0xDDDDDDDDDDDDDDDDULL;
    x ^= y & 0x5757575757575757ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x1C1C1C1C1C1C1C1CULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x4A4A4A4A4A4A4A4AULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x4242424242424242ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x6464646464646464ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xE0E0E0E0E0E0E0E0ULL;
    a1 = x;
    a1 ^= (x & 0xF0F0F0F0F0F0F0F0ULL) >> 4;
    a2 = ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((x & 0x3333333333333333ULL) << 2);
    a3 = x & a1;
    a3 ^= (a3 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a3 ^= (((x << 1) & a1) ^ ((a1 << 1) & x)) & 0xAAAAAAAAAAAAAAAAULL;
    a4 = a2 & a1;
    a4 ^= (a4 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a4 ^= (((a2 << 1) & a1) ^ ((a1 << 1) & a2)) & 0xAAAAAAAAAAAAAAAAULL;
    a5 = (a3 & 0xCCCCCCCCCCCCCCCCULL) >> 2;
    a3 ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCCCCCCCCCULL;
    a4 = a5 & 0x2222222222222222ULL;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x2222222222222222ULL;
    a3 ^= a4;
    a5 = a3 & 0xA0A0A0A0A0A0A0A0ULL;
    a5 |= a5 >> 1;
    a5 ^= (a3 << 1) & 0xA0A0A0A0A0A0A0A0ULL;
    a4 = a5 & 0xC0C0C0C0C0C0C0C0ULL;
    a6 = a4 >> 2;
    a4 ^= (a5 << 2) & 0xC0C0C0C0C0C0C0C0ULL;
    a5 = a6 & 0x2020202020202020ULL;
    a5 |= a5 >> 1;
    a5 ^= (a6 << 1) & 0x2020202020202020ULL;
    a4 |= a5;
    a3 ^= a4 >> 4;
    a3 &= 0x0F0F0F0F0F0F0F0FULL;
    a2 = a3;
    a2 ^= (a3 & 0x0C0C0C0C0C0C0C0CULL) >> 2;
    a4 = a3 & a2;
    a4 ^= (a4 & 0x0A0A0A0A0A0A0A0AULL) >> 1;
    a4 ^= (((a3 << 1) & a2) ^ ((a2 << 1) & a3)) & 0x0A0A0A0A0A0A0A0AULL;
    a5 = a4 & 0x0808080808080808ULL;
    a5 |= a5 >> 1;
    a5 ^= (a4 << 1) & 0x0808080808080808ULL;
    a4 ^= a5 >> 2;
    a4 &= 0x0303030303030303ULL;
    a4 ^= (a4 & 0x0202020202020202ULL) >> 1;
    a4 |= a4 << 2;
    a3 = a2 & a4;
    a3 ^= (a3 & 0x0A0A0A0A0A0A0A0AULL) >> 1;
    a3 ^= (((a2 << 1) & a4) ^ ((a4 << 1) & a2)) & 0x0A0A0A0A0A0A0A0AULL;
    a3 |= a3 << 4;
    a2 = ((a1 & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((a1 & 0x3333333333333333ULL) << 2);
    x = a1 & a3;
    x ^= (x & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    x ^= (((a1 << 1) & a3) ^ ((a3 << 1) & a1)) & 0xAAAAAAAAAAAAAAAAULL;
    a4 = a2 & a3;
    a4 ^= (a4 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a4 ^= (((a2 << 1) & a3) ^ ((a3 << 1) & a2)) & 0xAAAAAAAAAAAAAAAAULL;
    a5 = (x & 0xCCCCCCCCCCCCCCCCULL) >> 2;
    x ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCCCCCCCCCULL;
    a4 = a5 & 0x2222222222222222ULL;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x2222222222222222ULL;
    x ^= a4;
    y = ((x & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((x & 0x0101010101010101ULL) << 7);
    x &= 0x3939393939393939ULL;
    x ^= y & 0x3F3F3F3F3F3F3F3FULL;
    y = ((y & 0xFCFCFCFCFCFCFCFCULL) >> 2) | ((y & 0x0303030303030303ULL) << 6);
    x ^= y & 0x9797979797979797ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x9B9B9B9B9B9B9B9BULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x3C3C3C3C3C3C3C3CULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xDDDDDDDDDDDDDDDDULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x7272727272727272ULL;
    x ^= 0x6363636363636363ULL;
    *w = x;
  }

  void inv_sub_long(uint64_t *w) {
    uint64_t x, y, a1, a2, a3, a4, a5, a6;
    x = *w;
    x ^= 0x6363636363636363ULL;
    y = ((x & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((x & 0x0101010101010101ULL) << 7);
    x &= 0xFDFDFDFDFDFDFDFDULL;
    x ^= y & 0x5E5E5E5E5E5E5E5EULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xF3F3F3F3F3F3F3F3ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xF5F5F5F5F5F5F5F5ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x7878787878787878ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x7777777777777777ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x1515151515151515ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xA5A5A5A5A5A5A5A5ULL;
    a1 = x;
    a1 ^= (x & 0xF0F0F0F0F0F0F0F0ULL) >> 4;
    a2 = ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((x & 0x3333333333333333ULL) << 2);
    a3 = x & a1;
    a3 ^= (a3 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a3 ^= (((x << 1) & a1) ^ ((a1 << 1) & x)) & 0xAAAAAAAAAAAAAAAAULL;
    a4 = a2 & a1;
    a4 ^= (a4 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a4 ^= (((a2 << 1) & a1) ^ ((a1 << 1) & a2)) & 0xAAAAAAAAAAAAAAAAULL;
    a5 = (a3 & 0xCCCCCCCCCCCCCCCCULL) >> 2;
    a3 ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCCCCCCCCCULL;
    a4 = a5 & 0x2222222222222222ULL;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x2222222222222222ULL;
    a3 ^= a4;
    a5 = a3 & 0xA0A0A0A0A0A0A0A0ULL;
    a5 |= a5 >> 1;
    a5 ^= (a3 << 1) & 0xA0A0A0A0A0A0A0A0ULL;
    a4 = a5 & 0xC0C0C0C0C0C0C0C0ULL;
    a6 = a4 >> 2;
    a4 ^= (a5 << 2) & 0xC0C0C0C0C0C0C0C0ULL;
    a5 = a6 & 0x2020202020202020ULL;
    a5 |= a5 >> 1;
    a5 ^= (a6 << 1) & 0x2020202020202020ULL;
    a4 |= a5;
    a3 ^= a4 >> 4;
    a3 &= 0x0F0F0F0F0F0F0F0FULL;
    a2 = a3;
    a2 ^= (a3 & 0x0C0C0C0C0C0C0C0CULL) >> 2;
    a4 = a3 & a2;
    a4 ^= (a4 & 0x0A0A0A0A0A0A0A0AULL) >> 1;
    a4 ^= (((a3 << 1) & a2) ^ ((a2 << 1) & a3)) & 0x0A0A0A0A0A0A0A0AULL;
    a5 = a4 & 0x0808080808080808ULL;
    a5 |= a5 >> 1;
    a5 ^= (a4 << 1) & 0x0808080808080808ULL;
    a4 ^= a5 >> 2;
    a4 &= 0x0303030303030303ULL;
    a4 ^= (a4 & 0x0202020202020202ULL) >> 1;
    a4 |= a4 << 2;
    a3 = a2 & a4;
    a3 ^= (a3 & 0x0A0A0A0A0A0A0A0AULL) >> 1;
    a3 ^= (((a2 << 1) & a4) ^ ((a4 << 1) & a2)) & 0x0A0A0A0A0A0A0A0AULL;
    a3 |= a3 << 4;
    a2 = ((a1 & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((a1 & 0x3333333333333333ULL) << 2);
    x = a1 & a3;
    x ^= (x & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    x ^= (((a1 << 1) & a3) ^ ((a3 << 1) & a1)) & 0xAAAAAAAAAAAAAAAAULL;
    a4 = a2 & a3;
    a4 ^= (a4 & 0xAAAAAAAAAAAAAAAAULL) >> 1;
    a4 ^= (((a2 << 1) & a3) ^ ((a3 << 1) & a2)) & 0xAAAAAAAAAAAAAAAAULL;
    a5 = (x & 0xCCCCCCCCCCCCCCCCULL) >> 2;
    x ^= ((a4 << 2) ^ a4) & 0xCCCCCCCCCCCCCCCCULL;
    a4 = a5 & 0x2222222222222222ULL;
    a4 |= a4 >> 1;
    a4 ^= (a5 << 1) & 0x2222222222222222ULL;
    x ^= a4;
    y = ((x & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((x & 0x0101010101010101ULL) << 7);
    x &= 0xB5B5B5B5B5B5B5B5ULL;
    x ^= y & 0x4040404040404040ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x8080808080808080ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x1616161616161616ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xEBEBEBEBEBEBEBEBULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x9797979797979797ULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0xFBFBFBFBFBFBFBFBULL;
    y = ((y & 0xFEFEFEFEFEFEFEFEULL) >> 1) | ((y & 0x0101010101010101ULL) << 7);
    x ^= y & 0x7D7D7D7D7D7D7D7DULL;
    *w = x;
  }


  void shift_rows(uint64_t *state) {
    unsigned char s[4];
    unsigned char *s0;
    int r;
    s0 = (unsigned char *)state;
    for (r = 0; r < 4; r++) {
      s[0] = s0[0*4+r];
      s[1] = s0[1*4+r];
      s[2] = s0[2*4+r];
      s[3] = s0[3*4+r];
      s0[0*4+r] = s[(r+0)%4];
      s0[1*4+r] = s[(r+1)%4];
      s0[2*4+r] = s[(r+2)%4];
      s0[3*4+r] = s[(r+3)%4];
    }
  }

  void inv_shift_rows(uint64_t *state) {
    unsigned char s[4];
    unsigned char *s0;
    int r;
    s0 = (unsigned char *)state;
    for (r = 0; r < 4; r++) {
      s[0] = s0[0*4+r];
      s[1] = s0[1*4+r];
      s[2] = s0[2*4+r];
      s[3] = s0[3*4+r];
      s0[0*4+r] = s[(4-r)%4];
      s0[1*4+r] = s[(5-r)%4];
      s0[2*4+r] = s[(6-r)%4];
      s0[3*4+r] = s[(7-r)%4];
    }
  }


  void mix_columns(uint64_t *state) {
    uint64_t s1;
    uint64_t s;
    int c;
    for (c = 0; c < 2; c++) {
      s1 = state[c];
      s = s1;
      s ^= ((s & 0xFFFF0000FFFF0000ULL) >> 16) | ((s & 0x0000FFFF0000FFFFULL) << 16);
      s ^= ((s & 0xFF00FF00FF00FF00ULL) >>  8) | ((s & 0x00FF00FF00FF00FFULL) <<  8);
      s ^= s1;
      xtime_long(&s1);
      s ^= s1;
      reinterpret_cast<unsigned char *>(&s)[0] ^= reinterpret_cast<unsigned char *>(&s1)[1];
      reinterpret_cast<unsigned char *>(&s)[1] ^= reinterpret_cast<unsigned char *>(&s1)[2];
      reinterpret_cast<unsigned char *>(&s)[2] ^= reinterpret_cast<unsigned char *>(&s1)[3];
      reinterpret_cast<unsigned char *>(&s)[3] ^= reinterpret_cast<unsigned char *>(&s1)[0];
      reinterpret_cast<unsigned char *>(&s)[4] ^= reinterpret_cast<unsigned char *>(&s1)[5];
      reinterpret_cast<unsigned char *>(&s)[5] ^= reinterpret_cast<unsigned char *>(&s1)[6];
      reinterpret_cast<unsigned char *>(&s)[6] ^= reinterpret_cast<unsigned char *>(&s1)[7];
      reinterpret_cast<unsigned char *>(&s)[7] ^= reinterpret_cast<unsigned char *>(&s1)[4];
      state[c] = s;
    }
  }

  void inv_mix_columns(uint64_t *state) {
    uint64_t s1;
    uint64_t s;
    int c;
    for (c = 0; c < 2; c++) {
      s1 = state[c];
      s = s1;
      s ^= ((s & 0xFFFF0000FFFF0000ULL) >> 16) | ((s & 0x0000FFFF0000FFFFULL) << 16);
      s ^= ((s & 0xFF00FF00FF00FF00ULL) >>  8) | ((s & 0x00FF00FF00FF00FFULL) <<  8);
      s ^= s1;
      xtime_long(&s1);
      s ^= s1;
      reinterpret_cast<unsigned char *>(&s)[0] ^= reinterpret_cast<unsigned char *>(&s1)[1];
      reinterpret_cast<unsigned char *>(&s)[1] ^= reinterpret_cast<unsigned char *>(&s1)[2];
      reinterpret_cast<unsigned char *>(&s)[2] ^= reinterpret_cast<unsigned char *>(&s1)[3];
      reinterpret_cast<unsigned char *>(&s)[3] ^= reinterpret_cast<unsigned char *>(&s1)[0];
      reinterpret_cast<unsigned char *>(&s)[4] ^= reinterpret_cast<unsigned char *>(&s1)[5];
      reinterpret_cast<unsigned char *>(&s)[5] ^= reinterpret_cast<unsigned char *>(&s1)[6];
      reinterpret_cast<unsigned char *>(&s)[6] ^= reinterpret_cast<unsigned char *>(&s1)[7];
      reinterpret_cast<unsigned char *>(&s)[7] ^= reinterpret_cast<unsigned char *>(&s1)[4];
      xtime_long(&s1);
      s1 ^= ((s1 & 0xFFFF0000FFFF0000ULL) >> 16) | ((s1 & 0x0000FFFF0000FFFFULL) << 16);
      s ^= s1;
      xtime_long(&s1);
      s1 ^= ((s1 & 0xFF00FF00FF00FF00ULL) >>  8) | ((s1 & 0x00FF00FF00FF00FFULL) <<  8);
      s ^= s1;
      state[c] = s;
    }
  }


  void add_round_key(uint64_t *state, const uint64_t *w) {
    state[0] ^= w[0];
    state[1] ^= w[1];
  }


  static void rot_word(uint32_t *x) {
    unsigned char *w0;
    unsigned char tmp;
    w0 = (unsigned char *)x;
    tmp = w0[0];
    w0[0] = w0[1];
    w0[1] = w0[2];
    w0[2] = w0[3];
    w0[3] = tmp;
  }


  void key_expansion(aes_ctx *ctx, const uint8_t *key) {
    uint32_t words[(aes_max_rounds+1)*aes_round_key_len/aes_key_word_len];
    const int nk = ctx->kl_ / aes_key_word_len;
    std::memcpy(words, key, ctx->kl_);
    uint32_t rcon;
    std::memcpy(&rcon, "\1\0\0\0", sizeof(rcon));
    for (int i = nk; i < (ctx->nr_+1)*(aes_round_key_len/aes_key_word_len); ++i) {
      uint32_t temp = words[i-1];
      if (i % nk == 0) {
        rot_word(&temp);
        sub_word(&temp);
        temp ^= rcon;
        xtime_word(&rcon);
      } else if (nk == 8 && i % nk == 4) {
        sub_word(&temp);
      }
      words[i] = words[i-nk] ^ temp;
    }
    std::memcpy(ctx->round_keys_, words, (ctx->nr_+1)*aes_round_key_len);
  }

  void cipher(aes_ctx *ctx, const unsigned char *in, unsigned char *out) {
    uint64_t w[aes_max_rounds+1][aes_block_len/sizeof(uint64_t)];
    int i;

    std::memcpy(ctx->state_, in, aes_block_len);
    std::memcpy(w, ctx->round_keys_, (ctx->nr_+1)*aes_round_key_len);

    add_round_key(ctx->state_, w[0]);

    for (i = 1; i < ctx->nr_; i++) {
      sub_long(&ctx->state_[0]);
      sub_long(&ctx->state_[1]);
      shift_rows(ctx->state_);
      mix_columns(ctx->state_);
      add_round_key(ctx->state_, w[i]);
    }

    sub_long(&ctx->state_[0]);
    sub_long(&ctx->state_[1]);
    shift_rows(ctx->state_);
    add_round_key(ctx->state_, w[ctx->nr_]);

    memcpy(out, ctx->state_, 16);
  }

  void inv_cipher(aes_ctx *ctx, const unsigned char *in, unsigned char *out) {
    uint64_t w[aes_max_rounds+1][aes_block_len/sizeof(uint64_t)];
    int i;

    std::memcpy(ctx->state_, in, aes_block_len);
    std::memcpy(w, ctx->round_keys_, (ctx->nr_ + 1)*aes_round_key_len);

    add_round_key(ctx->state_, w[ctx->nr_]);

    for (i = ctx->nr_ - 1; i > 0; i--) {
      inv_shift_rows(ctx->state_);
      inv_sub_long(&ctx->state_[0]);
      inv_sub_long(&ctx->state_[1]);
      add_round_key(ctx->state_, w[i]);
      inv_mix_columns(ctx->state_);
    }

    inv_shift_rows(ctx->state_);
    inv_sub_long(&ctx->state_[0]);
    inv_sub_long(&ctx->state_[1]);
    add_round_key(ctx->state_, w[0]);

    memcpy(out, ctx->state_, 16);
  }


  /**
   * Initialize the context for the selected AES parameter set.
   * @param ctx context to initialize before encryption or decryption.
   * @param set AES parameter set to use.
   * @param key key containing 16, 24, or 32 bytes for AES-128, AES-192, or AES-256.
   * @throws std::invalid_argument for null pointers or an unsupported set.
   */
  void init(aes_set set, aes_ctx *ctx, const uint8_t *key) noexcept {
    switch (set) {
      case aes_set::aes128:
        ctx->nr_ = aes128_rounds;
        ctx->kl_ = aes128_key_len;
        break;
      case aes_set::aes192:
        ctx->nr_ = aes192_rounds;
        ctx->kl_ = aes192_key_len;
        break;
      case aes_set::aes256:
        ctx->nr_ = aes256_rounds;
        ctx->kl_ = aes256_key_len;
        break;
      default:
        return;
    }
    std::memset(ctx->round_keys_, 0, sizeof(ctx->round_keys_));
    std::memset(ctx->state_, 0, sizeof(ctx->state_));
    impl::key_expansion(ctx, key);
  }


  } // namespace impl


void encrypt(aes_set set, const uint8_t *key, uint8_t out[aes_block_len], const uint8_t in[aes_block_len]) noexcept {
  aes_ctx ctx;
  impl::init(set, &ctx, key);
  impl::cipher(&ctx, in, out);
}


void decrypt(aes_set set, const uint8_t *key, uint8_t out[aes_block_len], const uint8_t in[aes_block_len]) noexcept {
  aes_ctx ctx;
  impl::init(set, &ctx, key);
  impl::inv_cipher(&ctx, in, out);
}


} // namespace crypto::aes