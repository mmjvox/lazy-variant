#ifndef RESULTROW_H
#define RESULTROW_H


#include "DbVariant.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace LazyOrm {

class ResultRow : public std::unordered_map<uint,DbVariant>
{
private:
    std::shared_ptr<std::vector<std::string>> mColumnNamesPtr = std::make_shared<std::vector<std::string>>();
    const int findColumnIndex(const std::string &name) const;

public:
    using std::unordered_map<uint,DbVariant>::unordered_map;

    std::string toString() const;
    std::string toIndentedString() const;

    DbVariant value(const std::string key, const DbVariant dbVariant={}) const;
    void insert(const std::string key, const DbVariant dbVariant);
    DbVariant operator[](const std::string key) const;

    DbVariant at(unsigned long long columnIndex) const;
    DbVariant value(unsigned long long columnIndex) const;

    void setColumnNamesPtr(std::shared_ptr<std::vector<std::string> > newColumnNamesPtr);
};

}

#endif // RESULTROW_H
