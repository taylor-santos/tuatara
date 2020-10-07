#ifndef FLOAT_H
#define FLOAT_H

#include "expression.h"

namespace TuataraAST {

class Float final : public Expression {
private:
    double value;

protected:
    void json(std::ostream &os) const override;

public:
    Float(const yy::location &loc, double value);
    ~Float() override = default;
};

} // namespace TuataraAST

#endif // FLOAT_H
