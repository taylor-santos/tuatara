#include "type/type_exception.h"

#include <utility>

using namespace std;

namespace TypeChecker {

TypeException::TypeException(const std::string &msg, const yy::location &loc)
    : msgs_{make_pair(msg, loc)} {}

TypeException::TypeException(std::vector<std::pair<std::string, yy::location>> msgs)
    : msgs_{std::move(msgs)} {}

const vector<std::pair<std::string, yy::location>> &
TypeException::getMsgs() const {
    return msgs_;
}

} // namespace TypeChecker