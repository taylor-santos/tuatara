#include "type/type_exception.h"

#include "location.hh"

using std::pair, std::string, std::vector;

namespace TypeChecker {

TypeException::TypeException(const string &msg, const yy::location &loc)
    : msgs_{make_pair(msg, loc)} {}

TypeException::TypeException(vector<pair<string, yy::location>> msgs)
    : msgs_{move(msgs)} {}

TypeException::~TypeException() = default;

const vector<pair<string, yy::location>> &
TypeException::getMsgs() const {
    return msgs_;
}

} // namespace TypeChecker
