#include "ast/expression.h"

using namespace AST;

Expression::Expression(const yy::location &loc)
    : Statement(loc) {}
