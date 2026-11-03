cmake_minimum_required(VERSION 3.20)

project(sdk LANGUAGES CXX)


add_library(compat
  src/compat/byte_swap.cc
  src/compat/endian.cc
)
add_library(sdk::compat ALIAS compat)
target_include_directories(compat PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(compat PUBLIC cxx_std_23)


add_library(crypto
  src/crypto/ctops.cc
  src/crypto/rand.cc
  src/crypto/aes/aes.cc
  src/crypto/mlkem/mlkem.cc
  src/crypto/mlkem/ntt.cc
  src/crypto/mlkem/poly.cc
  src/crypto/mlkem/shake.cc
  src/crypto/sha/sha3.cc
)
add_library(sdk::crypto ALIAS crypto)
target_include_directories(crypto PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(crypto PUBLIC cxx_std_23)


add_library(sdk INTERFACE)
add_library(sdk::sdk ALIAS sdk)
target_link_libraries(sdk INTERFACE sdk::compat sdk::crypto)


option(SDK_BUILD_TESTS "Build SDK tests" OFF)
if(SDK_BUILD_TESTS)
  enable_testing()
  add_executable(sdk_cbc_test tests/crypto/mode/cbc.cc)
  target_link_libraries(sdk_cbc_test PRIVATE sdk::crypto)
  add_test(NAME sdk_cbc COMMAND sdk_cbc_test)

  add_executable(sdk_mlkem_test tests/crypto/mlkem/mlkem.cc)
  target_link_libraries(sdk_mlkem_test PRIVATE sdk::crypto)
  add_test(NAME sdk_mlkem COMMAND sdk_mlkem_test)
endif()