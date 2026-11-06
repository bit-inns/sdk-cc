#ifndef SDK_COMPAT_BYTE_SWAP_H
#define SDK_COMPAT_BYTE_SWAP_H

#include <cstdint>


namespace sdk::compat
{


uint16_t bswap16(uint16_t x);

uint32_t bswap32(uint32_t x);

uint64_t bswap64(uint64_t x);


} // namespace sdk::compat


#endif // #ifndef SDK_COMPAT_BYTE_SWAP_H