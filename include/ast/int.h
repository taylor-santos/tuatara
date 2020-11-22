#ifndef INT_H
#define INT_H

#include "expression.h"

namespace AST {

class Int final : public Expression {
public: // Methods
    Int(const yy::location &loc, int64_t value);
    ~Int() override = default;

private: // Fields
    int64_t value;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // INT_H
