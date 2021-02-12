#ifndef AST_LVALUE_H
#define AST_LVALUE_H

#include "ast/expression.h"

namespace AST {

class LValue : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<LValue>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit LValue(const yy::location &loc);
};

} // namespace AST

#endif // AST_LVALUE_H
