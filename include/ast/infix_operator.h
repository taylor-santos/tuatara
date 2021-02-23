#ifndef AST_INFIX_OPERATOR_H
#define AST_INFIX_OPERATOR_H

#include "ast/postfix_operator.h"
#include "ast/prefix_operator.h"

using namespace std;

namespace AST {

class InfixOperator final
    : public PrefixOperator
    , public PostfixOperator {
public: // Aliases
    using Ptr = std::unique_ptr<InfixOperator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    InfixOperator(
        const yy::location & loc,
        const std::string &  op,
        AST::Expression::Ptr lhs,
        AST::Expression::Ptr rhs);
    ~InfixOperator() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_INFIX_OPERATOR_H
