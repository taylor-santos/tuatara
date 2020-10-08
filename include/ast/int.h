#ifndef INT_H
#define INT_H

#include "expression.h"

namespace AST {

class Int final : public Expression {
private:
    int value;

protected:
    void json(std::ostream &os) const override;

public:
    Int(const yy::location &loc, int value);
    ~Int() override = default;
};

} // namespace AST

#endif // INT_H
