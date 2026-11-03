# sdk

C++ SDK requiring CMake 3.20+ and C++23.

## Modules

- **Crypto:** AES, SHA-3/SHAKE, ML-KEM, generic CBC, random bytes, and constant-time helpers.
- **Compat:** Byte swapping and endian utilities.
- **Serialization:** Header-only binary serialization and deserialization.

## Use with FetchContent

```cmake
cmake_minimum_required(VERSION 3.20)
project(my-app LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  sdk
  GIT_REPOSITORY <your-sdk-git-url>
  GIT_TAG <commit-or-tag>
)
FetchContent_MakeAvailable(sdk)

add_executable(my-app main.cpp)
target_link_libraries(my-app PRIVATE sdk::sdk)
```

Replace the URL and revision with your repository and a pinned commit or tag.
Link `sdk::sdk` for all modules, or `sdk::crypto` / `sdk::compat` individually.
Public headers use `sdk/` paths, such as `"sdk/crypto.h"` and `"sdk/serialization.h"`.

## Build and test

```sh
cmake -S . -B build -DSDK_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Contributing

Follow the [coding-style guide](docs/contribution/coding-style.md).