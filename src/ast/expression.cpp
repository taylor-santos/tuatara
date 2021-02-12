#include "ast/expression.h"

namespace AST {

Expression::Expression(const yy::location &loc)
    : Node(loc) {}

} // namespace AST
