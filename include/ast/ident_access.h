#ifndef AST_IDENT_ACCESS_H
#define AST_IDENT_ACCESS_H

#include <optional>

#include "ast/lvalue.h"

#include "type/type.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

namespace AST {

class Node;

class IdentAccess final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<IdentAccess>;
    using Vec = std::vector<Ptr>;

public: // Methods
    IdentAccess(
        const yy::location &loc,
        Expression::Ptr     expr,
        const yy::location &idLoc,
        std::string         ident);
    ~IdentAccess() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr                expr_;
    yy::location                   idLoc_;
    std::string                    ident_;
    std::optional<Expression::Ptr> state_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_IDENT_ACCESS_H
