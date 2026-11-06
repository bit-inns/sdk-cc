#ifndef SDK_CRYPTO_CTOPS_H
#define SDK_CRYPTO_CTOPS_H

#include <cstdint>
#include <cstdlib>


namespace sdk::crypto
{

  
/**
 * Compare two byte buffers without early exit.
 * @param a first buffer, containing at least len readable bytes.
 * @param b second buffer, containing at least len readable bytes.
 * @param len number of bytes to compare.
 * @return 0 if the buffers are equal, otherwise 1.
 */
int ccmp(const uint8_t *a, const uint8_t *b, size_t len);

/**
 * Conditionally copy bytes using a bit mask.
 * @param in source buffer, containing at least len readable bytes.
 * @param out destination buffer, containing at least len writable bytes.
 * @param len number of bytes to process.
 * @param b selection flag: 1 copies in to out; 0 preserves out.
 */
void cmov_uint8(const uint8_t *in, uint8_t *out, size_t len, uint8_t b);

/**
 * Conditionally assign a 16-bit value using a bit mask.
 * @param in source value.
 * @param out pointer to the destination value.
 * @param b selection flag: 1 assigns in to *out; 0 preserves *out.
 */
void cmov_int16(int16_t in, int16_t *out, uint16_t b);


} // namespace sdk::crypto


#endif // #ifndef SDK_CRYPTO_CTOPS_H