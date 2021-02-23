#ifndef AST_LAMBDA_H
#define AST_LAMBDA_H

#include <optional>

#include "ast/block.h"

#include "pattern/pattern.h"

#include "type/type.h"

namespace AST {

class Lambda final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Lambda>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Lambda(
        const yy::location &                  loc,
        Pattern::Pattern::Vec                 args,
        std::optional<TypeChecker::Type::Ptr> retType,
        Expression::Ptr                       body);
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Pattern::Pattern::Vec                 args_;
    std::optional<TypeChecker::Type::Ptr> retType_;
    Expression::Ptr                       body_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_LAMBDA_H
