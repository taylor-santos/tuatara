#ifndef LVALUE_H
#define LVALUE_H

#include "expression.h"

namespace AST {

class LValue : public Expression {
protected:
    explicit LValue(const yy::location &loc);
};

} // namespace AST

#endif // LVALUE_H
