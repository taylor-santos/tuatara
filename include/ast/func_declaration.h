#ifndef AST_FUNC_DECLARATION_H
#define AST_FUNC_DECLARATION_H

#include <optional>
#include <vector>

#include "ast/declaration.h"

namespace yy {
class location;
} // namespace yy
namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace Pattern {
class Pattern;
} // namespace Pattern

namespace AST {

class FuncDeclaration : public Declaration {
public: // Methods
    FuncDeclaration(
        const yy::location &                              loc,
        const yy::location &                              varLoc,
        std::string                                       variable,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::optional<std::shared_ptr<TypeChecker::Type>> retType = {});
    ~FuncDeclaration() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const std::vector<std::unique_ptr<Pattern::Pattern>> &   getArgs() const;
    [[nodiscard]] const std::optional<std::shared_ptr<TypeChecker::Type>> &getRetType() const;
    std::shared_ptr<TypeChecker::Type> getDeclTypeImpl(TypeChecker::Context &ctx) override;
    TypeChecker::Context &             calculateContext(TypeChecker::Context &outerCtx);

private: // Fields
    std::vector<std::unique_ptr<Pattern::Pattern>>    args_;
    std::optional<std::shared_ptr<TypeChecker::Type>> retType_;
    std::unique_ptr<TypeChecker::Context>             implCtx_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FUNC_DECLARATION_H
