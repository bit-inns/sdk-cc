#include "sdk/crypto/ctops.h"


namespace sdk::crypto
{


int ccmp(const uint8_t *a, const uint8_t *b, size_t len) {
  uint8_t r = 0;
  for (size_t i = 0; i < len; ++i) {
    r |= a[i] ^ b[i];
  }
  return (-(uint64_t)r) >> 63;

}

void cmov_uint8(const uint8_t *in, uint8_t *out, size_t len, uint8_t b) {
  b = -b;
  for (int i = 0; i < len; ++i) {
    out[i] ^= b & (out[i] ^ in[i]);
  }
}

void cmov_int16(int16_t in, int16_t *out, uint16_t b) {
  b = -b;
  *out ^= b & ((*out) ^ in);
}


} // namespace sdk::crypto