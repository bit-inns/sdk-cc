#include "sdk/crypto/rand.h"

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#else
#include <fcntl.h>
#include <errno.h>
#endif // #ifdef _WIN32

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#endif // #if defined(__linux__) || defined(__APPLE__)


namespace sdk::crypto
{


void gen_rand_bytes(uint8_t *out, std::size_t len) {
#ifdef _WIN32
  // TODO: Implement random-byte generation on Windows.
#elif defined(__linux__) || defined(__APPLE__)
  static int fd = -1;
  ssize_t r;
  while (fd == -1) {
    fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1 && errno == EINTR) continue;
    else if (fd == -1) abort();
  }
  while (len > 0) {
    r = read(fd, out, len);
    if (r == -1 && errno == EINTR) continue;
    else if (r == -1) abort();
    out += r;
    len -= r;
  }
#endif // #ifdef _WIN32
}


} // namespace sdk::crypto