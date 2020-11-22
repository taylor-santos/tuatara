#ifndef FLOAT_H
#define FLOAT_H

#include "expression.h"

namespace AST {

class Float final : public Expression {
public: // Methods
    Float(const yy::location &loc, double value);
    ~Float() override = default;

private: // Fields
    double value;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // FLOAT_H
