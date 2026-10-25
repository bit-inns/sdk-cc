#ifndef COMPAT_BYTE_SWAP_H
#define COMPAT_BYTE_SWAP_H

#include <cstdint>


namespace compat
{


uint16_t bswap16(uint16_t x);

uint32_t bswap32(uint32_t x);

uint64_t bswap64(uint64_t x);


} // namespace compat


#endif // #ifndef COMPAT_BYTE_SWAP_H