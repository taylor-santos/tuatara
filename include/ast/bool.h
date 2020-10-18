#ifndef BOOL_H
#define BOOL_H

#include "expression.h"

namespace AST {

class Bool final : public Expression {
private:
    bool value;

protected:
    void json(std::ostream &os) const override;

public:
    Bool(const yy::location &loc, bool value);
    ~Bool() override = default;
};

} // namespace AST

#endif // BOOL_H
