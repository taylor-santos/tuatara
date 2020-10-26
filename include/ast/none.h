#ifndef NONE_H
#define NONE_H

#include "expression.h"

namespace AST {

class None final : public Expression {
private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    None(const yy::location &loc);
    ~None() override = default;
};

} // namespace AST

#endif // NONE_H
