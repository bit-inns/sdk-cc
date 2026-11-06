#ifndef SDK_COMPAT_ENDIAN_H
#define SDK_COMPAT_ENDIAN_H

#include <bit>
#include <cstdint>

#include "sdk/compat/byte_swap.h"


namespace sdk::compat
{


uint16_t htole16(uint16_t x);

uint16_t htobe16(uint16_t x);

uint16_t betoh16(uint16_t x);

uint16_t letoh16(uint16_t x);


uint32_t htole32(uint32_t x);

uint32_t htobe32(uint32_t x);

uint32_t betoh32(uint32_t x);

uint32_t letoh32(uint32_t x);


} // namespace sdk::compat


#endif // #ifndef SDK_COMPAT_ENDIAN_H