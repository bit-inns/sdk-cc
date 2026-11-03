#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <vector>


#include "sdk/crypto/mlkem/mlkem.h"


inline void print_bytes(const uint8_t *in, std::size_t len) {
  for (std::size_t i = 0; i < len; ++i) {
    std::cout << std::format("{:02x}", *(in+i));
  }
  std::cout << std::endl;
}

inline void print_numbers(const uint8_t *in, std::size_t len) {
  for (std::size_t i = 0; i < len; ++i) {
    std::cout << std::format("{:02d} ", *(in + i));
  }
  std::cout << std::endl;
}


uint8_t drb[crypto::mlkem::seed_len]{
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
  0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
  0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20
};


int main(int argc, char* argv[]) {
  using namespace crypto::mlkem;

  const auto set = mlkem_set::mlkem_1024;
  mlkem_ctx ctx;
  make_mlkem_ctx(set, &ctx);

  std::vector<uint8_t> public_key(ctx.ek_len_);
  std::vector<uint8_t> private_key(ctx.dk_len_);

  mlkem_key_gen(set, public_key.data(), private_key.data());

  print_bytes(public_key.data(), public_key.size());
  print_bytes(private_key.data(), private_key.size());

  uint8_t alice_secret_key[mlkem_msg_len];
  std::vector<uint8_t> cipher(ctx.compressed_poly_len_+ctx.compressed_poly_vec_len_);
  mlkem_encap(set, cipher.data(), alice_secret_key, public_key.data());

  uint8_t bob_secret_key[mlkem_msg_len];
  mlkem_decap(set, bob_secret_key, cipher.data(), private_key.data());

  std::cout << std::endl << "Ali secret key" << std::endl;
  print_bytes(alice_secret_key, sizeof(alice_secret_key));
  std::cout << std::endl << "Bob secret key" << std::endl;
  print_bytes(bob_secret_key, sizeof(bob_secret_key));

  return 0;
}