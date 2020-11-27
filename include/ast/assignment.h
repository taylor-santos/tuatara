#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "expression.h"
#include "lvalue.h"

namespace AST {

class Assignment final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Assignment>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Assignment(const yy::location &loc, LValue::Ptr lhs, Expression::Ptr rhs);

private: // Fields
    LValue::Ptr     lhs;
    Expression::Ptr rhs;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // ASSIGNMENT_H
