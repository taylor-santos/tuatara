#ifndef CALL_H
#define CALL_H

#include "lvalue.h"

#include <memory>
#include <vector>

namespace AST {

class Call final : public LValue {
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
