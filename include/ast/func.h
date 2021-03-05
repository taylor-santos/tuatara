#ifndef AST_FUNC_H
#define AST_FUNC_H

#include <optional>
#include <vector>

#include "ast/declaration.h"

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

class Func final : public Declaration {
public: // Methods
    Func(
        const yy::location &                              loc,
        const yy::location &                              varLoc,
        std::string                                       variable,
        std::vector<std::unique_ptr<Pattern::Pattern>>    args,
        std::unique_ptr<Block>                            body,
        std::optional<std::shared_ptr<TypeChecker::Type>> retType = {});
    ~Func() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::vector<std::unique_ptr<Pattern::Pattern>>       args_;
    std::optional<std::shared_ptr<TypeChecker::Type>>    retType_;
    std::unique_ptr<Block>                               body_;
    std::optional<std::unique_ptr<TypeChecker::Context>> implCtx_;

private: // Methods
    void                               json(std::ostream &os) const override;
    std::shared_ptr<TypeChecker::Type> getDeclTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FUNC_H
