# LazyVariant

LazyVariant is a small C++ value/result utility library extracted from LazyORM.

It provides a database-friendly variant type, row/result containers, and small helper utilities for converting values to strings, numbers, booleans, quoted SQL-safe fragments, and JSON-like output.

> Current namespace: `LazyOrm`  
> Library target: `lazyvariant`  
> Required C++ standard: C++20

## What is included

LazyVariant currently contains four public components:

| Component | Purpose |
|---|---|
| `LazyOrm::DbVariant` | A database-oriented variant type for strings, integers, floating-point values, booleans, and empty values. |
| `LazyOrm::DbList` | A helper variant for SQL-style list values such as `(...)`. |
| `LazyOrm::ResultRow` | A row container mapping column names to `DbVariant` values. |
| `LazyOrm::Result` | A result-set container made of multiple `ResultRow` objects, plus metadata like errors, affected rows, insert id, and column names. |

## Why this exists

LazyORM used to contain both ORM/query logic and low-level value/result containers. Pulling LazyVariant out makes sense because these classes are useful outside the ORM layer:

- standalone database abstraction layers
- SQL client wrappers
- result serialization
- query builders
- tests and mocks
- small internal tools that need typed database values without depending on the full ORM

Keeping this as a separate library is cleaner than dragging the whole ORM into projects that only need value conversion and result containers.

## Installation

### Build from source

```bash
git clone git@github.com:mmjvox/lazy-variant.git lazyvariant
cd lazyvariant
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This builds the static library:

```text
liblazyvariant.a
```

### Build tests

Tests are disabled by default.

```bash
cmake -S . -B build -Dtests=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build
```

The test suite uses Catch2 through CMake `FetchContent`.

### Install

```bash
cmake --install build --prefix /usr/local
```

Headers are installed under:

```text
include/LazyVariant
```

Library output is installed under the configured CMake library directory, usually:

```text
lib/liblazyvariant.a
```

## CMake usage

After installation:

```cmake
find_package(lazyvariant REQUIRED)

add_executable(app main.cpp)
target_include_directories(app PRIVATE ${lazyvariant_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${lazyvariant_LIBRARIES})
```

Direct subdirectory usage:

```cmake
add_subdirectory(lazyvariant)

target_link_libraries(app PRIVATE lazyvariant)
```

## Basic usage

```cpp
#include <DbVariant.h>

    using namespace LazyOrm;

    DbVariant id = 42;
    DbVariant name = "Ilya";
    DbVariant active = true;

    std::cout << id.toLongLong() << "\n";      // 42
    std::cout << name.toString() << "\n";     // Ilya
    std::cout << active.toBool() << "\n";     // 1
```

## Result usage

```cpp
#include <Result.h>
#include <ResultRow.h>

    using namespace LazyOrm;

    Result result;
    
    auto affectedRows = result.affectedRows();    
    std::cout << "Affected rows: " << affectedRows << "\n";
    
    
    auto rowsCount = result.size();    
    std::cout << "Count of rows: " << rowsCount << "\n";
    
    if(rowsCount>0){
        ResultRow row = result[0];
        std::cout << "name: " << row["name"] << "\n";
    }

    std::cout << result.toString() << "\n";

```

Example output:

```
Affected rows: 0
Count of rows: 1
name: Kimia
```

```json
[{"active":true,"name":"Kimia","id":1}]
```

`ResultRow` is based on `std::unordered_map`, so key ordering in string output is not guaranteed.

## DbVariant supported value types

`DbVariant` is based on `std::variant` and supports:

- `std::monostate`
- `std::string`
- unsigned integers: `unsigned short`, `unsigned int`, `unsigned long`, `unsigned long long`
- signed integers: `short`, `int`, `long`, `long long`
- floating-point numbers: `float`, `double`, `long double`
- `bool`

## Useful conversions

```cpp
LazyOrm::DbVariant value = "123";

value.toString();      // "123"
value.toLongLong();    // 123
value.toULongLong();   // 123
value.toLongDouble();  // 123.0
value.toBool();        // true
value.typeName();      // "string"
```

## String helpers

```cpp
LazyOrm::DbVariant text = "  Hello World  ";

text.trim();           // "Hello World"
text.toLowerString();  // "  hello world  "
text.toUpperString();  // "  HELLO WORLD  "
text.contains("World");
text.startsWith("  He");
```

## SQL-oriented helpers

```cpp
LazyOrm::DbVariant name = "O'Reilly";
LazyOrm::DbVariant column = "users.name";

name.setQuote();        // 'O\'Reilly'
column.setBackTick();   // `users`.`name`
column.setDoubleQuote();
```

`setQuote()`, `setBackTick()`, and `setDoubleQuote()` are helper functions for building SQL fragments. They are not a replacement for parameterized queries. Do not use them as your only SQL injection defense in external-input paths.

## DbList usage

```cpp
#include <DbList.h>

LazyOrm::DbList ids = {1, 2, 3};
LazyOrm::DbList names = {"ali", "reza", "sara"};

ids.toString();    // [no'](1,2,3)
names.toString();  // [no']('ali','reza','sara')
```

`DbList` stores its generated list representation as a string inside `DbVariant`.

## API documentation

See:

- [`docs/quick-start.md`](docs/quick-start.md)
- [`docs/api.md`](docs/api.md)
- [`docs/installation.md`](docs/installation.md)
- [`docs/notes.md`](docs/notes.md)

## Important cleanup before public release

This extraction is almost done, but not fully clean yet. Fix these before tagging a serious release:

1. Rename the namespace from `LazyOrm` to either `LazyVariant` or keep `LazyOrm` intentionally and document it as compatibility mode.
2. Rename CMake variables like `LazyORM_HEADERS` to `LazyVariant_HEADERS`.
3. Fix install config destination from `lib/cmake/lazyorm` to `lib/cmake/lazyvariant`.
4. Consider renaming the CMake option `tests` to something less generic like `LAZYVARIANT_BUILD_TESTS`.
5. Consider exporting a proper CMake target such as `LazyVariant::lazyvariant` instead of only variables.

Shipping it without these fixes works technically, but it looks unfinished.

## License

See [`LICENSE`](LICENSE).
