#ifndef AST_FUNC_IMPL_H
#define AST_FUNC_IMPL_H

#include "ast/block.h"
#include "ast/func_declaration.h"

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

class Node;

class FuncImpl final : public FuncDeclaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncImpl>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncImpl(
        const yy::location &                              loc,
        const yy::location &                              varLoc,
        std::string                                       variable,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::unique_ptr<Block>                            body,
        std::optional<std::unique_ptr<TypeChecker::Type>> retType = {});
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Block> body_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FUNC_IMPL_H
