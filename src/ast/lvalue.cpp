#include "ast/lvalue.h"

using namespace AST;

LValue::LValue(const yy::location &loc)
    : Expression(loc) {}
