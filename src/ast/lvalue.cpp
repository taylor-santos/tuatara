#include "ast/lvalue.h"

namespace AST {

LValue::LValue(const yy::location &loc)
    : Expression(loc) {}

} // namespace AST
