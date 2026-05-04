#include "Result.h"
#include "ResultRow.h"
#include <algorithm>

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
    if(index < Base::size()){
        return Base::at(index);
    }

    ResultRow row;
    row.setColumnNamesPtr(mColumnNamesPtr);
    return row;
}

// push_back rvalue
inline void Result::push_back(ResultRow &&value) {
    value.setColumnNamesPtr(mColumnNamesPtr);
    Base::push_back(std::move(value));
}

inline std::vector<ResultRow>::iterator
Result::insert(const_iterator position, ResultRow &&value) {
    value.setColumnNamesPtr(mColumnNamesPtr);
    return Base::insert(position, std::move(value));
}

template<typename InputIt>
std::vector<ResultRow>::iterator
Result::insert(const_iterator position, InputIt first, InputIt last) {
    std::vector<ResultRow> temp(first, last);
    for (auto &v : temp) {
        v.setColumnNamesPtr(mColumnNamesPtr);
    }
    return Base::insert(position, temp.begin(), temp.end());
}

inline std::vector<ResultRow>::iterator
Result::insert(const_iterator position, std::initializer_list<ResultRow> ilist) {
    std::vector<ResultRow> temp(ilist);
    for (auto &v : temp) {
        v.setColumnNamesPtr(mColumnNamesPtr);
    }
    return Base::insert(position, temp.begin(), temp.end());
}

template<typename InputIt>
void Result::assign(InputIt first, InputIt last) {
    std::vector<ResultRow> temp(first, last);
    for (auto &v : temp) {
        v.setColumnNamesPtr(mColumnNamesPtr);
    }
    Base::assign(temp.begin(), temp.end());
}

inline void Result::assign(std::initializer_list<ResultRow> ilist) {
    std::vector<ResultRow> temp(ilist);
    for (auto &v : temp) {
        v.setColumnNamesPtr(mColumnNamesPtr);
    }
    Base::assign(temp.begin(), temp.end());
}

const ResultRow &Result::operator[](size_t index) const {
    const Base* basePtr = this;
    auto &row = const_cast<ResultRow&>((*basePtr)[index]);
    row.setColumnNamesPtr(mColumnNamesPtr);
    return row;
}

ResultRow &Result::at(size_t index) {
    auto &row = Base::at(index);
    row.setColumnNamesPtr(mColumnNamesPtr);
    return row;
}

const ResultRow &Result::at(size_t index) const {
    auto &row = const_cast<ResultRow&>(Base::at(index));
    row.setColumnNamesPtr(mColumnNamesPtr);
    return row;
}

ResultRow &Result::operator[](size_t index) {
    Base* basePtr = this;
    auto &row = (*basePtr)[index];
    row.setColumnNamesPtr(mColumnNamesPtr);
    return row;
}

template<class... Args>
ResultRow& Result::emplace_back(Args&&... args) {
    ResultRow row(std::forward<Args>(args)...);
    row.setColumnNamesPtr(mColumnNamesPtr);
    return Base::emplace_back(std::move(row));
}

template<class... Args>
std::vector<ResultRow>::iterator
Result::emplace(const_iterator position, Args&&... args) {
    ResultRow row(std::forward<Args>(args)...);
    row.setColumnNamesPtr(mColumnNamesPtr);
    return Base::emplace(position, std::move(row));
}

const int Result::findColumnIndex(const std::string &name) const
{
    const auto it= std::find(mColumnNamesPtr->begin(), mColumnNamesPtr->end(), name);
    if (it == mColumnNamesPtr->end()) {
        return -1;
    }
    return std::distance(mColumnNamesPtr->begin(), it);

}

}
