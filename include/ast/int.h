#ifndef INT_H
#define INT_H

#include "expression.h"

namespace AST {

class Int final : public Expression {
private:
    int64_t value;

protected:
    void json(std::ostream &os) const override;

public:
    Int(const yy::location &loc, int64_t value);
    ~Int() override = default;
};

} // namespace AST

#endif // INT_H