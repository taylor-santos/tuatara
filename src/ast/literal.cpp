#include "ast/literal.h"

namespace AST {

Literal::Literal(const yy::location &loc)
    : Expression(loc) {}

} // namespace AST
