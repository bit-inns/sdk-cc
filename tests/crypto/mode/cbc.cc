#include "sdk/crypto/mode/cbc.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>

#include "sdk/crypto/aes/aes.h"


void print_bytes(const uint8_t *in, std::size_t len) {
  for (std::size_t i = 0; i < len; ++i) {
    std::cout << std::format("{:02x}", *(in+i));
  }
  std::cout << std::endl;
}


void check(bool result) {
  if (!result) {
    std::fputs("CBC check failed\n", stderr);
    std::abort();
  }
}

void test_aes() {
  // AES-128 CBC known-answer example, four blocks.
  const uint8_t key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
  };
  const std::array<uint8_t, 16> initial_iv = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };
  const std::array<uint8_t, 64> plain = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
  };
  const std::array<uint8_t, 64> expected = {
    0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
    0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
    0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
    0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
  };
  auto encrypt = [&](uint8_t *out, const uint8_t *in) {
    crypto::aes::encrypt(crypto::aes::aes_set::aes128, key, out, in);
  };
  auto decrypt = [&](uint8_t *out, const uint8_t *in) {
    crypto::aes::decrypt(crypto::aes::aes_set::aes128, key, out, in);
  };
  std::cout << "AES-128 CBC key" << std::endl;
  print_bytes(key, sizeof(key));
  std::cout << "IV" << std::endl;
  print_bytes(initial_iv.data(), initial_iv.size());
  std::cout << "Plaintext" << std::endl;
  print_bytes(plain.data(), plain.size());

  auto iv = initial_iv;
  std::array<uint8_t, 64> output;
  check(crypto::mode::cbc_encrypt<16>(encrypt, output.data(), plain.data(), plain.size(), iv.data()));
  std::cout << "Ciphertext" << std::endl;
  print_bytes(output.data(), output.size());
  check(output == expected);
  check(std::equal(iv.begin(), iv.end(), expected.end()-16));
  iv = initial_iv;
  check(crypto::mode::cbc_decrypt<16>(decrypt, output.data(), expected.data(), expected.size(), iv.data()));
  std::cout << "Decrypted plaintext" << std::endl;
  print_bytes(output.data(), output.size());
  check(output == plain);
  check(std::equal(iv.begin(), iv.end(), expected.end()-16));

  output = plain;
  iv = initial_iv;
  check(crypto::mode::cbc_encrypt<16>(encrypt, output.data(), output.data(), 16, iv.data()));
  check(crypto::mode::cbc_encrypt<16>(encrypt, output.data()+16, output.data()+16, 48, iv.data()));
  check(output == expected);
  iv = initial_iv;
  check(crypto::mode::cbc_decrypt<16>(decrypt, output.data(), output.data(), 32, iv.data()));
  check(crypto::mode::cbc_decrypt<16>(decrypt, output.data()+32, output.data()+32, 32, iv.data()));
  check(output == plain);
}

void test_generic() {
  // A three-byte reversible test cipher verifies CBC has no AES or block-size dependency.
  unsigned int calls = 0;
  auto cipher = [&](uint8_t *out, const uint8_t *in) {
    ++calls;
    check(out != in);
    for (std::size_t i = 0; i < 3; ++i) {
      out[i] = in[i] ^ 0x80;
    }
  };
  std::array<uint8_t, 3> iv = {1, 2, 3};
  const auto initial_iv = iv;
  std::array<uint8_t, 6> data = {4, 5, 6, 7, 8, 9};
  const auto plain = data;
  const std::array<uint8_t, 6> expected = {133, 135, 133, 2, 15, 12};
  check(crypto::mode::cbc_encrypt<3>(cipher, data.data(), data.data(), data.size(), iv.data()));
  std::cout << "Three-byte test cipher CBC ciphertext" << std::endl;
  print_bytes(data.data(), data.size());
  check(data == expected);
  iv = initial_iv;
  check(crypto::mode::cbc_decrypt<3>(cipher, data.data(), data.data(), data.size(), iv.data()));
  std::cout << "Decrypted plaintext" << std::endl;
  print_bytes(data.data(), data.size());
  check(data == plain);

  for (bool decrypt : {false, true}) {
    for (unsigned int invalid = 0; invalid < 4; ++invalid) {
      iv = initial_iv;
      const auto before_calls = calls;
      uint8_t *out = invalid == 1 ? nullptr : data.data();
      const uint8_t *in = invalid == 2 ? nullptr : data.data();
      uint8_t *chain = invalid == 3 ? nullptr : iv.data();
      const std::size_t len = invalid == 0 ? 5 : 6;
      bool result = decrypt ? crypto::mode::cbc_decrypt<3>(cipher, out, in, len, chain)
                            : crypto::mode::cbc_encrypt<3>(cipher, out, in, len, chain);
      check(!result && calls == before_calls && data == plain && iv == initial_iv);
    }
  }
  const auto before_calls = calls;
  check(crypto::mode::cbc_encrypt<3>(cipher, nullptr, nullptr, 0, nullptr));
  check(crypto::mode::cbc_decrypt<3>(cipher, nullptr, nullptr, 0, nullptr));
  check(calls == before_calls);
}


int main() {
  test_aes();
  test_generic();
}