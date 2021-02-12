#include "ast/literal.h"

#include "json.h"

namespace AST {

Literal::Literal(const yy::location &loc)
    : Expression(loc) {}

} // namespace AST
