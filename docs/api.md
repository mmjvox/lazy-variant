# LazyVariant API

## Namespace

All public classes currently live in:

```cpp
namespace LazyOrm
```

Because this library was extracted from LazyORM, the namespace has not been renamed yet.

## Header overview

```cpp
#include <DbVariant.h>
#include <DbList.h>
#include <Result.h>
#include <ResultRow.h>
```

## DbVariant

`DbVariant` is the core value container.

```cpp
class DbVariant : public std::variant<
    std::monostate,
    std::string,
    UnsignedIntegerVariant,
    SignedIntegerVariant,
    SignedFloatingPointVariant,
    bool
>
```

### Type aliases

```cpp
using UnsignedIntegerVariant = std::variant<
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long
>;

using SignedIntegerVariant = std::variant<
    short,
    int,
    long,
    long long
>;

using SignedFloatingPointVariant = std::variant<
    float,
    double,
    long double
>;
```

### Construction

```cpp
LazyOrm::DbVariant empty;
LazyOrm::DbVariant text = "hello";
LazyOrm::DbVariant number = 42;
LazyOrm::DbVariant decimal = 3.14;
LazyOrm::DbVariant flag = true;
```

### Assignment helper

```cpp
DbVariant& set(DbVariant newVariant);
```

Example:

```cpp
LazyOrm::DbVariant value = 1;
value.set("changed");
```

### Type inspection

```cpp
std::string typeName() const;
bool isMonostate();
bool isNumeric() const;
```

`typeName()` returns one of:

```text
string
uint
int
float
boolean
```

For `std::monostate`, it returns an empty string.

### Conversion methods

```cpp
std::string toString() const;
unsigned long long toULongLong() const;
long long toLongLong() const;
long double toLongDouble() const;
bool toBool() const;
```

Example:

```cpp
LazyOrm::DbVariant value = "42";

value.toString();      // "42"
value.toLongLong();    // 42
value.toULongLong();   // 42
value.toLongDouble();  // 42.0
value.toBool();        // true
```

### String helpers

```cpp
std::string toLowerString() const;
std::string toUpperString() const;
bool empty();
const size_t lenght() const;
bool contains(std::string substr) const;
bool startsWith(std::string prefix, bool caseSensitive = true) const;
std::string trim() const;
```

Note: the method name is currently spelled `lenght()`, not `length()`.

### SQL-fragment helpers

```cpp
std::string setQuote() const;
std::string setBackTick() const;
std::string setDoubleQuote() const;
std::string toCleanString() const;
bool isUpdate() const;
```

Example:

```cpp
LazyOrm::DbVariant value = "O'Reilly";
value.setQuote(); // 'O\'Reilly'
```

Use these helpers for controlled SQL-generation internals only. For external input, prefer prepared statements and bound parameters.

### Type mutation helpers

```cpp
DbVariant alterStringToBestMatchType();
DbVariant alterStringToNumber();
```

Example:

```cpp
LazyOrm::DbVariant value = "123";
auto converted = value.alterStringToBestMatchType();
```

### Comparisons

```cpp
bool operator==(std::monostate);
bool operator==(std::string);
bool operator==(unsigned long long);
bool operator==(long long);
bool operator==(long double);
bool operator==(bool);
```

Example:

```cpp
LazyOrm::DbVariant value = 42;

if (value == 42LL) {
    // matched
}
```

### Alias helper

```cpp
static const std::string as(const std::string& columnName, const std::string& asName);
```

Example:

```cpp
LazyOrm::DbVariant::as("users.name", "user_name");
```

## DbList

`DbList` is a small helper that stores a SQL-style list representation inside a `DbVariant`.

```cpp
class DbList : public DbVariant
```

### Constructors

```cpp
DbList();
DbList(std::initializer_list<DbVariant> input);
```

### Methods

```cpp
void setList(std::vector<DbVariant> list);
```

Example:

```cpp
LazyOrm::DbList ids = {1, 2, 3};
ids.toString(); // [no'](1,2,3)
```

## ResultRow

`ResultRow` represents a single result row.

```cpp
class ResultRow : public std::unordered_map<std::string, DbVariant>
```

### Serialization

```cpp
std::string toString() const;
std::string toIndentedString() const;
```

Example:

```cpp
LazyOrm::ResultRow row;
row.insert("name", "Kimia");
row.insert("age", 22);

row.toString();
```

### Value access

```cpp
DbVariant value(const std::string key, const DbVariant dbVariant = {}) const;
DbVariant operator[](const std::string key) const;
DbVariant at(unsigned long long columnIndex) const;
DbVariant value(unsigned long long columnIndex) const;
```

Example:

```cpp
row.value("name").toString();
row["age"].toLongLong();
row.at(0);
```

Important: `ResultRow` inherits from `std::unordered_map`, so index-based access depends on unordered-map iteration order. Do not rely on it for stable column ordering unless you control insertion and runtime behavior very carefully.

### Insert/update

```cpp
void insert(const std::string key, const DbVariant dbVariant);
```

This uses `insert_or_assign`, so inserting an existing key updates the value.

## Result

`Result` represents a result set.

```cpp
class Result : public std::vector<ResultRow>
```

### Serialization

```cpp
std::string toString() const;
std::string toIndentedString() const;
```

### Error handling

```cpp
bool hasError() const;
void setError(std::string errorMessage);
std::string errorMessage() const;
```

Example:

```cpp
LazyOrm::Result result;
result.setError("connection failed");

if (result.hasError()) {
    auto error = result.errorMessage();
}
```

### Metadata

```cpp
size_t affectedRows() const;
void setAffectedRows(size_t affectedRows);

unsigned long long insertId() const;
void setInsertId(unsigned long long insertId);

void setColumnNames(const std::vector<std::string>& columnNames);
std::vector<std::string> columnNames();
std::string columnName(size_t index);
size_t columnsSize() const;
```

### Row access

```cpp
ResultRow value(unsigned long long index);
```

Example:

```cpp
LazyOrm::ResultRow first = result.value(0);
```

If the index is out of range, an empty `ResultRow` is returned.
