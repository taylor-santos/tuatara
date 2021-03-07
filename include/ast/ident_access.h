#ifndef AST_IDENT_ACCESS_H
#define AST_IDENT_ACCESS_H

#include <optional>

#include "ast/lvalue.h"

namespace TypeChecker {
class Type;
class Context;
} // namespace TypeChecker

namespace AST {

class IdentAccess final : public LValue {
public: // Methods
    IdentAccess(
        const yy::location &        loc,
        std::unique_ptr<Expression> expr,
        const yy::location &        idLoc,
        std::string                 ident);
    ~IdentAccess() override;
    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> expr_;
    yy::location                idLoc_;
    std::string                 ident_;

private: // Methods
    void
    json(std::ostream &os) const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_IDENT_ACCESS_H
