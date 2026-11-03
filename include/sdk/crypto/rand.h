#ifndef CRYPTO_RAND_H
#define CRYPTO_RAND_H

#include <cstddef>
#include <cstdint>
#include <cstdlib>


namespace crypto
{


void gen_rand_bytes(uint8_t *out, std::size_t len);


} // namespace crypto


#endif // #ifndef CRYPTO_RAND_H