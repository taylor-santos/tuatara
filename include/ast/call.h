#ifndef CALL_H
#define CALL_H

#include "lvalue.h"

namespace AST {

class Call final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Call>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Call(const yy::location &loc, Expression::Ptr function, Expression::Vec args);
    ~Call() override = default;

private: // Fields
    Expression::Ptr function;
    Expression::Vec args;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // CALL_H
