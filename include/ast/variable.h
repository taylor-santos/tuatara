#ifndef VARIABLE_H
#define VARIABLE_H

#include "lvalue.h"

namespace AST {

class Variable final : public LValue {
private: // Fields
    std::string name;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    Variable(const yy::location &loc, std::string name);
    ~Variable() override = default;
};

} // namespace AST

#endif // VARIABLE_H
