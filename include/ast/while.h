#ifndef AST_WHILE_H
#define AST_WHILE_H

#include "ast/block.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class While final : public Expression {
public: // Methods
    While(const yy::location &loc, std::unique_ptr<Expression> cond, std::unique_ptr<Block> block);
    ~While() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> cond_;
    std::unique_ptr<Block>      block_;

private: // Methods
    void                               json(std::ostream &os) const override;
    std::shared_ptr<TypeChecker::Type> getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_WHILE_H
