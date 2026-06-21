# Installation

## Requirements

- CMake 3.15 or newer
- C++20 compiler
- Catch2 is downloaded automatically only when tests are enabled

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Build with tests

```bash
cmake -S . -B build -Dvariant_tests=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

## Install

```bash
cmake --install build --prefix /usr/local
```

Installed files are expected to include:

```text
/usr/local/include/LazyVariant/DbVariant.h
/usr/local/include/LazyVariant/DbList.h
/usr/local/include/LazyVariant/Result.h
/usr/local/include/LazyVariant/ResultRow.h
/usr/local/lib/liblazyvariant.a
```

## Use from CMake

```cmake
find_package(lazyvariant REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE lazyvariant::lazyvariant)
```

The package exports the `lazyvariant::lazyvariant` target, so consumers automatically inherit the correct include directories and definitions simply by linking to it.
