#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "expression.h"

#include <memory>

namespace TuataraAST {

class Assignment : public Expression {
protected:
    std::string                 variable;
    std::unique_ptr<Expression> value;

public:
    Assignment(const yy::location &loc, std::string variable, std::unique_ptr<Expression> value);

protected:
    void json(std::ostream &os) const override;
};

} // namespace TuataraAST

#endif // ASSIGNMENT_H
