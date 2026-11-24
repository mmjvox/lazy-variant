#include "DbList.h"
#include <utility>

namespace LazyOrm {

DbList::DbList()
{

}

DbList::DbList(std::initializer_list<DbVariant> input)
{
    setList( {input.begin(), input.end()} );
}

void DbList::setList(std::vector<DbVariant> list)
{
    emplace<DbListType>(std::move(list));
}

} // namespace LazyOrm
