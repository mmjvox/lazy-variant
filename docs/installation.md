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
cmake -S . -B build -Dtests=ON -DCMAKE_BUILD_TYPE=Debug
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
target_include_directories(app PRIVATE ${lazyvariant_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${lazyvariant_LIBRARIES})
```

## Current CMake caveat

The project currently generates `lazyvariantConfig.cmake`, but the install destination in `CMakeLists.txt` still points to `lib/cmake/lazyorm`.

That is a leftover from LazyORM and should be changed to:

```cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lazyvariant
```

The variable names `LazyORM_HEADERS` should also be renamed to `LazyVariant_HEADERS` for clarity.
