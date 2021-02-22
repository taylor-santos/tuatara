#include "ast/operator.h"

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
