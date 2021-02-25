#ifndef AST_FUNC_IMPL_H
#define AST_FUNC_IMPL_H

#include <vector>

#include "ast/func_declaration.h"

namespace Pattern {
class Pattern;
} // namespace Pattern
namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Type;
class Context;
} // namespace TypeChecker

namespace AST {

class Block;

class FuncImpl final : public FuncDeclaration {
public: // Methods
    FuncImpl(
        const yy::location &                              loc,
        const yy::location &                              varLoc,
        std::string                                       variable,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::unique_ptr<Block>                            body,
        std::optional<std::unique_ptr<TypeChecker::Type>> retType = {});
    ~FuncImpl() override;
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
