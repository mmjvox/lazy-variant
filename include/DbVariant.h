#ifndef DBVARIANT_H
#define DBVARIANT_H

#include <string>
#include <variant>
#include <map>
#include <list>
#include <vector>

#ifdef __cpp_lib_byte
#include <cstddef>
#endif

namespace LazyOrm {

enum Filters
{
    None = -10,
    ORDERBY=1,
    ORDERBY_DESC,
    ORDERBY_ASC,
    LIMIT,
    GROUPBY
};

enum NestedWhere
{
    NNone = -10,
    WHERE = 0,
    AND = 1,
    OR,
    NOT,
};

enum class Having
{
    NNone = -10,
    HAVING = 0,
    AND = 1,
    OR,
    NOT,
};

#ifdef __cpp_lib_byte
using ByteType = std::byte;
#else
using ByteType = uint8_t;
#endif


using UnsignedIntegerVariant = std::variant<unsigned short, unsigned int, unsigned long, unsigned long long>;
using SignedIntegerVariant = std::variant<short, int, long, long long>;
using SignedFloatingPointVariant = std::variant<float, double, long double>;
using BlobType = std::vector<ByteType>;

class DbVariant;

class DbListType
{
    std::vector<DbVariant> mValues;

public:
    DbListType() = default;
    DbListType(std::vector<DbVariant> values);

    size_t size() const;
    bool empty() const;
    const DbVariant &at(size_t index) const;
    std::vector<DbVariant>::const_iterator begin() const;
    std::vector<DbVariant>::const_iterator end() const;

    bool operator<(const DbListType &other) const;
    bool operator==(const DbListType &other) const;
};

class DbVariant : public std::variant<std::monostate,std::string,UnsignedIntegerVariant,SignedIntegerVariant,SignedFloatingPointVariant,bool,BlobType,DbListType>
{
#ifdef DEBUG_MODE
public:
#else
private:
#endif
    UnsignedIntegerVariant toUnsignedIntegerVariant() const;
    bool isUnsignedIntegerVariant() const;
    unsigned long long toUnsignedInteger() const;
    bool can_UInt_Fit_Int(unsigned long long value) const;
    bool can_UInt_Fit_Float(unsigned long long value) const;

    SignedIntegerVariant toSignedIntegerVariant() const;
    bool isSignedIntegerVariant() const;
    long long toSignedInteger() const;
    bool can_Int_Fit_UInt(long long value) const;
    bool can_Int_Fit_Float(long long value) const;

    SignedFloatingPointVariant toSignedFloatingPointVariant() const;
    bool isSignedFloatingPointVariant() const;
    long double toSignedFloatingPoint() const;
    bool can_Float_Fit_Int(long double value) const;
    bool can_Float_Fit_UInt(long double value) const;

    std::string toFixedString(long double value) const;
    std::string toFixedString(const std::string &str) const;

    long long safeStringToLL(std::string value) const;
    unsigned long long safeStringToULL(std::string value) const;
    long double safeStringToLD(std::string value) const;
    bool isScientific(const std::string &value) const;
    bool isScientific(const double &value) const;
    std::string escapeSingleQuotes(const std::string& in) const;



public:
    using Base = std::variant<std::monostate,std::string,UnsignedIntegerVariant,SignedIntegerVariant,SignedFloatingPointVariant,bool,BlobType,DbListType>;
    using Base::variant;

    DbVariant& set(DbVariant newVariant);

    std::string typeName() const;
    bool isMonostate();
    std::string toString() const;
    unsigned long long toULongLong() const;
    long long toLongLong() const;
    long double toLongDouble() const;
    bool toBool() const;
    bool isBlob() const;
    BlobType toBlob() const;
    bool isList() const;
    DbListType toList() const;

    std::string toLowerString() const;
    std::string toUpperString() const;

    bool empty();
    const size_t length() const;
    bool contains(std::string substr) const;
    bool startsWith(std::string prefix, bool caseSensitive=true) const;
    std::string trim() const;

    std::string setQuote() const;
    std::string setBackTick() const;
    std::string setDoubleQuote() const;
    std::string toCleanString() const;
    bool isUpdate() const;

    DbVariant alterStringToBestMatchType();
    DbVariant alterStringToNumber();

    bool isNumeric() const;

    const std::map<std::string, LazyOrm::Filters> getFiltersToStringMap() const;

    bool operator==(std::monostate);
    bool operator==(std::string);
    bool operator==(unsigned long long);
    bool operator==(long long);
    bool operator==(long double);
    bool operator==(bool);
    bool operator==(BlobType);

    static const std::string as(const std::string &columnName, const std::string &asName);
};

typedef std::pair<std::string,LazyOrm::DbVariant> pair;

}

#endif // DBVARIANT_H
