#ifndef VARIABLE_H
#define VARIABLE_H

#include "lvalue.h"

namespace AST {

class Variable final : public LValue {
public: // Methods
    Variable(const yy::location &loc, std::string name);
    ~Variable() override = default;

private: // Fields
    std::string name;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // VARIABLE_H
