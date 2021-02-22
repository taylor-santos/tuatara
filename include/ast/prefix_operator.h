#ifndef AST_PREFIX_OPERATOR_H
#define AST_PREFIX_OPERATOR_H

#include "ast/operator.h"

namespace AST {

class PrefixOperator : virtual public Operator {
public: // Aliases
    using Ptr = std::unique_ptr<PrefixOperator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    PrefixOperator(const yy::location &loc, std::string op, AST::Expression::Ptr rhs);
    ~PrefixOperator() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getRhs() const;
    void                            json(std::ostream &os) const override;

private: // Fields
    Expression::Ptr rhs_;
};

} // namespace AST

#endif // AST_PREFIX_OPERATOR_H
