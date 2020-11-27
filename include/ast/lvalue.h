#ifndef LVALUE_H
#define LVALUE_H

#include "expression.h"

namespace AST {

class LValue : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<LValue>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit LValue(const yy::location &loc);
};

} // namespace AST

#endif // LVALUE_H
