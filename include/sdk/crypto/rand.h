#ifndef SDK_CRYPTO_RAND_H
#define SDK_CRYPTO_RAND_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>


namespace sdk::crypto
{


void gen_rand_bytes(uint8_t *out, std::size_t len);


} // namespace sdk::crypto


#endif // #ifndef SDK_CRYPTO_RAND_H