#include "ast/operator.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Operator::Operator(const yy::location &loc, string op)
    : LValue(loc)
    , op_{move(op)} {}

const string &
Operator::getOp() const {
    return op_;
}

} // namespace AST
