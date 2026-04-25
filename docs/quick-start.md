# LazyVariant Quick Start

## 1. Add the library

Using `add_subdirectory` is the fastest way while developing:

```cmake
add_subdirectory(external/lazyvariant)

target_link_libraries(my_app PRIVATE lazyvariant)
```

Or use an installed package:

```cmake
find_package(lazyvariant REQUIRED)

target_include_directories(my_app PRIVATE ${lazyvariant_INCLUDE_DIRS})
target_link_libraries(my_app PRIVATE ${lazyvariant_LIBRARIES})
```

## 2. Store typed values

```cpp
#include <DbVariant.h>
#include <iostream>

int main() {
    LazyOrm::DbVariant name = "Ilya";
    LazyOrm::DbVariant age = 28;
    LazyOrm::DbVariant score = 19.5;
    LazyOrm::DbVariant active = true;

    std::cout << name.toString() << "\n";
    std::cout << age.toLongLong() << "\n";
    std::cout << score.toLongDouble() << "\n";
    std::cout << active.toBool() << "\n";
}
```

## 3. Convert strings to best matching numeric type

```cpp
LazyOrm::DbVariant raw = "123";
LazyOrm::DbVariant converted = raw.alterStringToBestMatchType();

converted.typeName(); // usually "int" or "uint", depending on the value
```

## 4. Build result rows

```cpp
#include <ResultRow.h>

LazyOrm::ResultRow row;
row.insert("id", 1);
row.insert("name", "Kimia");
row.insert("active", true);

row.value("name").toString();
row["active"].toBool();
```

## 5. Build result sets

```cpp
#include <Result.h>
#include <ResultRow.h>

LazyOrm::Result result;

LazyOrm::ResultRow row;
row.insert("id", 1);
row.insert("name", "Kimia");

result.push_back(row);
result.setAffectedRows(1);
result.setInsertId(10);
result.setColumnNames({"id", "name"});

result.value(0).value("name").toString();
result.toString();
result.toIndentedString();
```

## 6. Use DbList for SQL-style lists

```cpp
#include <DbList.h>

LazyOrm::DbList ids = {1, 2, 3};
LazyOrm::DbList names = {"ali", "reza"};

ids.toString();   // [no'](1,2,3)
names.toString(); // [no']('ali','reza')
```

## Practical warning

LazyVariant has SQL-fragment helpers, but it is not a query-safety layer. For real database input, prefer bound parameters in your database driver.
