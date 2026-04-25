# Notes and Cleanup Checklist

LazyVariant has been extracted from LazyORM, but a few LazyORM leftovers are still visible.

## Must fix before a clean release

### 1. Namespace decision

Current namespace:

```cpp
namespace LazyOrm
```

Options:

- Keep `LazyOrm` for backward compatibility.
- Rename to `LazyVariant` for a clean standalone project.
- Add an alias temporarily:

```cpp
namespace LazyVariant = LazyOrm;
```

Best practical approach: keep `LazyOrm` for one compatibility release, document it, then rename with a migration note.

### 2. CMake variable names

Current names like this are leftovers:

```cmake
set(LazyORM_HEADERS ...)
```

Better:

```cmake
set(LazyVariant_HEADERS ...)
```

### 3. CMake install destination

Current config install destination points to LazyORM:

```cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lazyorm
```

Should be:

```cmake
DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lazyvariant
```

### 4. Test option name

Current:

```cmake
option(tests "build tests" OFF)
```

This is too generic. Use:

```cmake
option(LAZYVARIANT_BUILD_TESTS "Build LazyVariant tests" OFF)
```

### 5. Export proper CMake target

Current package config exposes variables:

```cmake
lazyvariant_INCLUDE_DIRS
lazyvariant_LIBRARIES
```

Better modern CMake should export:

```cmake
LazyVariant::lazyvariant
```

Then users can write:

```cmake
target_link_libraries(app PRIVATE LazyVariant::lazyvariant)
```

## API caveats

### `lenght()` typo

The public method is currently:

```cpp
lenght()
```

This typo is now part of the API. Fixing it directly is a breaking change. Safer migration:

```cpp
size_t length() const;
size_t lenght() const; // deprecated compatibility wrapper
```

### `ResultRow::value(key, fallback)` ignores fallback

The signature accepts a fallback value:

```cpp
DbVariant value(const std::string key, const DbVariant dbVariant = {}) const;
```

But the implementation currently returns `{}` when the key is missing, not `dbVariant`.

Expected implementation should probably be:

```cpp
if (value == end()) {
    return dbVariant;
}
```

### `ResultRow` ordering is not stable

`ResultRow` inherits from `std::unordered_map`. This means JSON-like output order is not guaranteed. Tests that assert exact string output can become fragile.

### SQL helpers are not enough for security

Functions like `setQuote()` are useful for internal query generation, but they are not a serious replacement for prepared statements.
