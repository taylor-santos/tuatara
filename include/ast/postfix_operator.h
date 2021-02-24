#ifndef AST_POSTFIX_OPERATOR_H
#define AST_POSTFIX_OPERATOR_H

#include "ast/operator.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {
class Node;

class PostfixOperator : virtual public Operator {
public: // Aliases
    using Ptr = std::unique_ptr<PostfixOperator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    PostfixOperator(const yy::location &loc, std::string op, AST::Expression::Ptr lhs);
    ~PostfixOperator() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getLhs() const;

private: // Fields
    Expression::Ptr lhs_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_POSTFIX_OPERATOR_H
