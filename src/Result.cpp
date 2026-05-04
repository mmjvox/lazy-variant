#include "Result.h"
#include "ResultRow.h"

namespace LazyOrm {

std::string Result::errorMessage() const
{
    return mErrorMessage;
}

std::string Result::toString() const
{
    std::string retStr="[";
    bool first = true;
    for(const auto &value : *this){
        if (!first) {
            retStr.append(",");
        }
        first = false;
        retStr.append(value.toString());
    }
    retStr.append("]");
    return retStr;
}

std::string Result::toIndentedString() const
{
    std::string retStr="[";
    bool first = true;
    for(const auto &value : *this){
        if (!first) {
            retStr.append(",");
        }
        first = false;
        retStr.append(value.toIndentedString());
    }
    retStr.append("]");
    return retStr;
}

bool Result::hasError() const {
    return !mErrorMessage.empty();
}

void Result::setError(std::string errorMessage){
    mErrorMessage = errorMessage;
}

size_t Result::affectedRows() const {
    return mAffectedRows;
}

void Result::setAffectedRows(size_t affectedRows)
{
    mAffectedRows = affectedRows;
}

void Result::setColumnNames(const std::vector<std::string> &columnNames)
{
    mColumnNamesPtr = std::make_shared<std::vector<std::string>>(columnNames);
}

void Result::setColumnNames(std::shared_ptr<std::vector<std::string> > columnNamesPtr)
{
    mColumnNamesPtr = columnNamesPtr;
}

std::vector<std::string> Result::columnNames() const{
    return *mColumnNamesPtr;
}

std::string Result::columnName(size_t index) const{
    if(mColumnNamesPtr->size()>index)
        return mColumnNamesPtr->at(index);
    return "";
}

size_t Result::columnsSize() const{
    return mColumnNamesPtr->size();
}

unsigned long long Result::insertId() const {
    return mInsertId;
}

void Result::setInsertId(unsigned long long insertId) {
    mInsertId = insertId;
}

ResultRow Result::value(unsigned long long index)
{
    if(index < std::vector<ResultRow>::size()){
        return std::vector<ResultRow>::at(index);
    }

    ResultRow row;
    row.setColumnNamesPtr(this->mColumnNamesPtr);
    return row;
}

}
