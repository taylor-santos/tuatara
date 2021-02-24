#ifndef AST_FUNC_DECLARATION_H
#define AST_FUNC_DECLARATION_H

#include <optional>

#include "ast/declaration.h"

namespace TypeChecker {
class Type;
}

namespace Pattern {
class Pattern;
}

namespace AST {

class FuncDeclaration : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncDeclaration(
        const yy::location &                              loc,
        const yy::location &                              varLoc,
        std::string                                       variable,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::optional<std::unique_ptr<TypeChecker::Type>> retType = {});
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const std::vector<std::unique_ptr<Pattern::Pattern>> &   getArgs() const;
    [[nodiscard]] const std::optional<std::unique_ptr<TypeChecker::Type>> &getRetType() const;

private: // Fields
    std::vector<std::unique_ptr<Pattern::Pattern>>    args_;
    std::optional<std::unique_ptr<TypeChecker::Type>> retType_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FUNC_DECLARATION_H
