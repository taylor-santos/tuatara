#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "expression.h"
#include "lvalue.h"

#include <memory>

namespace AST {

class Assignment final : public Expression {
private: // Fields
    std::unique_ptr<LValue>     lhs;
    std::unique_ptr<Expression> rhs;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    Assignment(
        const yy::location &        loc,
        std::unique_ptr<LValue>     lhs,
        std::unique_ptr<Expression> rhs);
};

} // namespace AST

#endif // ASSIGNMENT_H
