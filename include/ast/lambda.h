#ifndef AST_LAMBDA_H
#define AST_LAMBDA_H

#include <optional>
#include <vector>

#include "ast/expression.h"

namespace Pattern {
class Pattern;
} // namespace Pattern
namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;
}

namespace AST {

class Lambda final : public Expression {
public: // Methods
    Lambda(
        const yy::location &                              loc,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::optional<std::unique_ptr<TypeChecker::Type>> retType,
        std::unique_ptr<Expression>                       body);
    ~Lambda() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::vector<std::unique_ptr<Pattern::Pattern>>    args_;
    std::optional<std::unique_ptr<TypeChecker::Type>> retType_;
    std::unique_ptr<Expression>                       body_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_LAMBDA_H
