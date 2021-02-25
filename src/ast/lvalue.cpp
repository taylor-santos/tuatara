#include "ast/lvalue.h"

namespace yy {
class location;
} // namespace yy

namespace AST {

LValue::LValue(const yy::location &loc)
    : Expression(loc) {}

LValue::~LValue() = default;

} // namespace AST
