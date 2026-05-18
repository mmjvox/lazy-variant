#ifndef RESULT_H
#define RESULT_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace LazyOrm {

class ResultRow;

class Result : public std::vector<ResultRow>
{
private:
    using Base = std::vector<ResultRow>;
    std::string mErrorMessage;
    size_t mAffectedRows;
    unsigned long long mInsertId=0;
    std::shared_ptr<std::vector<std::string>> mColumnNamesPtr = std::make_shared<std::vector<std::string>>();
    const int findColumnIndex(const std::string &name) const;

public:
    using std::vector<ResultRow>::vector;
    using std::vector<ResultRow>::assign;
    using std::vector<ResultRow>::emplace;
    using std::vector<ResultRow>::emplace_back;
    using std::vector<ResultRow>::push_back;
    using std::vector<ResultRow>::insert;
    using std::vector<ResultRow>::operator[];
    using std::vector<ResultRow>::at;

    std::string toString() const;
    std::string toIndentedString() const;
    bool hasError() const;
    void setError(std::string errorMessage);
    size_t affectedRows() const;
    void setAffectedRows(size_t affectedRows);
    std::vector<std::string> columnNames() const;
    std::string columnName(size_t index) const;
    size_t columnsSize() const;
    unsigned long long insertId() const;
    void setInsertId(unsigned long long insertId);
    void setColumnNames(const std::vector<std::string> &columnNames);
    void setColumnNames(std::shared_ptr<std::vector<std::string>> columnNamesPtr);
    std::string errorMessage() const;

    ResultRow value(unsigned long long index);

// reimplementations
    // rvalue
    void push_back(ResultRow &&value);
    std::vector<ResultRow>::iterator insert(const_iterator position, ResultRow &&value);

    // range / initializer_list
    template<typename InputIt>
    std::vector<ResultRow>::iterator insert(const_iterator position, InputIt first, InputIt last);

    std::vector<ResultRow>::iterator insert(const_iterator position, std::initializer_list<ResultRow> ilist);

    template<typename InputIt>
    void assign(InputIt first, InputIt last);

    void assign(std::initializer_list<ResultRow> ilist);

    // emplace
    template<class... Args>
    ResultRow& emplace_back(Args&&... args);

    template<class... Args>
    std::vector<ResultRow>::iterator emplace(const_iterator position, Args&&... args);

    ResultRow& operator[](size_t index);

    const ResultRow& operator[](size_t index) const;

    ResultRow& at(size_t index);

    const ResultRow& at(size_t index) const;
};

}

#endif // RESULT_H
