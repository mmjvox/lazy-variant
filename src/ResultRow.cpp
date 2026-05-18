#include "ResultRow.h"
#include <algorithm>

namespace LazyOrm {

std::string ResultRow::toString() const
{
    std::string retStr="{";
    bool first = true;
    for(const auto &[key, value] : *this){

        if (!first) {
            retStr.append(",");
        }
        first = false;

        const auto &keyStr = mColumnNamesPtr->at(key);
        retStr.append("\""+keyStr+"\"");
        retStr.append(":");
        // value.alterStringToBestMatchType();
        if(value.typeName()=="string"){
            retStr.append("\""+value.toString()+"\"");
        } else {
            retStr.append(value.toString());
        }
    }
    retStr.append("}");
    return retStr;
}

std::string ResultRow::toIndentedString() const
{
    std::string retStr="\n  {";
    bool first = true;
    for(const auto &[key, value] : *this){

        if (!first) {
            retStr.append(",");
        }
        first = false;

        const auto &keyStr = mColumnNamesPtr->at(key);
        retStr.append("\n   \""+keyStr+"\"");
        retStr.append(":");
        // value.alterStringToBestMatchType();
        if(value.typeName()=="string"){
            retStr.append("\""+value.toString()+"\"");
        } else {
            retStr.append(value.toString());
        }
    }
    retStr.append("\n  }");
    return retStr;
}

LazyOrm::DbVariant ResultRow::value(const std::string key, const DbVariant dbVariant) const
{
    const auto &index = findColumnIndex(key);
    if(index<0){
        return dbVariant;
    }
    return this->at(index);
}

void ResultRow::insert(const std::string key, const DbVariant dbVariant)
{
    const auto &index = findColumnIndex(key);
    if(index<0){
        const auto &newIndex = mColumnNamesPtr->size();
        mColumnNamesPtr->push_back(key);
        insert_or_assign(newIndex, dbVariant);
    }
}

LazyOrm::DbVariant ResultRow::operator[](const std::string key) const
{
    return this->value(key);
}

LazyOrm::DbVariant ResultRow::at(unsigned long long columnIndex) const
{
    unsigned long long column = -1;
    DbVariant dbVariant;
    for(const auto &[key, value] : *this){
        column++;
        if(column==columnIndex){
            dbVariant = value;
            break;
        }
    }
    return dbVariant;
}

LazyOrm::DbVariant ResultRow::value(unsigned long long columnIndex) const
{
    return at(columnIndex);
}

void ResultRow::setColumnNamesPtr(std::shared_ptr<std::vector<std::string> > newColumnNamesPtr)
{
    mColumnNamesPtr = newColumnNamesPtr;
}

const int ResultRow::findColumnIndex(const std::string &name) const
{
    const auto it= std::find(mColumnNamesPtr->begin(), mColumnNamesPtr->end(), name);
    if (it == mColumnNamesPtr->end()) {
        return -1;
    }
    return std::distance(mColumnNamesPtr->begin(), it);

}


}
