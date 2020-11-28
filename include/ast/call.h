#ifndef AST_CALL_H
#define AST_CALL_H

#include "lvalue.h"

#include <optional>

namespace AST {

class Call final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Call>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Call(const yy::location &loc, Expression::Ptr function, std::optional<Expression::Ptr> arg);
    ~Call() override = default;

private: // Fields
    Expression::Ptr                function;
    std::optional<Expression::Ptr> arg;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_CALL_H
