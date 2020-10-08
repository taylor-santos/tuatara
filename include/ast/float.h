#ifndef FLOAT_H
#define FLOAT_H

#include "expression.h"

namespace AST {

class Float final : public Expression {
private:
    double value;

protected:
    void json(std::ostream &os) const override;

public:
    Float(const yy::location &loc, double value);
    ~Float() override = default;
};

} // namespace AST

#endif // FLOAT_H
