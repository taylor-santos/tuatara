#ifndef AST_LVALUE_H
#define AST_LVALUE_H

#include "ast/expression.h"

namespace yy {
class location;
} // namespace yy

namespace AST {

class LValue : public Expression {
public: // Methods
    ~LValue() override;

protected: // Methods
    explicit LValue(const yy::location &loc);
};

} // namespace AST

#endif // AST_LVALUE_H
