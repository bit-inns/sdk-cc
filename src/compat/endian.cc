#include "sdk/compat/endian.h"


namespace compat
{


uint16_t htole16(uint16_t x) {
  if constexpr (std::endian::native == std::endian::little) return x;
  return bswap16(x);
}

uint16_t htobe16(uint16_t x) {
  if constexpr (std::endian::native == std::endian::big) return x;
  return bswap16(x);
}

uint16_t betoh16(uint16_t x) {
  if constexpr (std::endian::native == std::endian::big) return x;
  return bswap16(x);
}

uint16_t letoh16(uint16_t x) {
  if constexpr (std::endian::native == std::endian::little) return x;
  return bswap16(x);
}


uint32_t htole32(uint32_t x) {
  if constexpr (std::endian::native == std::endian::little) return x;
  return bswap32(x);
}

uint32_t htobe32(uint32_t x) {
  if constexpr (std::endian::native == std::endian::big) return x;
  return bswap32(x);
}

uint32_t betoh32(uint32_t x) {
  if constexpr (std::endian::native == std::endian::big) return x;
  return bswap32(x);
}

uint32_t letoh32(uint32_t x) {
  if constexpr (std::endian::native == std::endian::little) return x;
  return bswap32(x);
}


} // namespace compat