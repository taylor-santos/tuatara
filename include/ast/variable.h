#ifndef VARIABLE_H
#define VARIABLE_H

#include "lvalue.h"

namespace AST {

class Variable : public LValue {
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
