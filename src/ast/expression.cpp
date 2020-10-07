#include "ast/expression.h"

using namespace TuataraAST;

Expression::Expression(const yy::location &loc)
    : Statement(loc) {}
