#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "expression.h"
#include "lvalue.h"

#include <memory>

namespace AST {

class Assignment final : public Expression {
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
