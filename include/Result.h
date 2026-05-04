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
    std::string mErrorMessage;
    size_t mAffectedRows;
    unsigned long long mInsertId=0;
    std::shared_ptr<std::vector<std::string>> mColumnNamesPtr = std::make_shared<std::vector<std::string>>();

public:
    using std::vector<ResultRow>::vector;

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
};

}

#endif // RESULT_H
