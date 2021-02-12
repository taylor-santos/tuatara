#include "ast/operator.h"

#include "json.h"

using namespace std;

namespace AST {

Operator::Operator(const yy::location &loc, string op)
    : LValue(loc)
    , op_{move(op)} {}

void
Operator::walk(const Func &fn) const {
    LValue::walk(fn);
}

const string &
Operator::getOp() const {
    return op_;
}

} // namespace AST
