#ifndef VARIABLE_H
#define VARIABLE_H

#include "expression.h"

namespace AST {

class Variable : public Expression {
protected:
    std::string name;

protected:
    void json(std::ostream &os) const override;

public:
    Variable(const yy::location &loc, std::string name);
    ~Variable() override = default;
};

} // namespace AST

#endif // VARIABLE_H
