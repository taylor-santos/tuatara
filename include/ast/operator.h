#ifndef AST_OPERATOR_H
#define AST_OPERATOR_H

#include "lvalue.h"

#include <optional>

namespace AST {

class Operator final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Operator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Operator(const yy::location &loc, std::string op, Expression::Ptr lhs, Expression::Ptr rhs);
    ~Operator() override = default;

private: // Fields
    std::string     op;
    Expression::Ptr lhs;
    Expression::Ptr rhs;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_OPERATOR_H
