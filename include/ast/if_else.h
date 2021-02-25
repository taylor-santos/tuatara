#ifndef AST_IF_ELSE_H
#define AST_IF_ELSE_H

#include "ast/if.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class IfElse final : public If {
public: // Methods
    IfElse(
        const yy::location &        loc,
        std::unique_ptr<Expression> cond,
        std::unique_ptr<Block>      block,
        std::unique_ptr<Block>      elseBlock);
    ~IfElse() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Block> elseBlock_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_IF_ELSE_H
