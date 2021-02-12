#ifndef AST_POSTFIX_OPERATOR_H
#define AST_POSTFIX_OPERATOR_H

#include "ast/operator.h"

namespace AST {

class PostfixOperator : virtual public Operator {
public: // Aliases
    using Ptr = std::unique_ptr<PostfixOperator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    PostfixOperator(const yy::location &loc, std::string op, AST::Expression::Ptr lhs);
    ~PostfixOperator() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getLhs() const;
    void                            json(std::ostream &os) const override;

private: // Fields
    Expression::Ptr lhs_;
};

} // namespace AST

#endif // AST_POSTFIX_OPERATOR_H
