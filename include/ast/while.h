#ifndef AST_WHILE_H
#define AST_WHILE_H

#include "ast/block.h"

namespace AST {

class While final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<While>;
    using Vec = std::vector<Ptr>;

public: // Methods
    While(const yy::location &loc, Expression::Ptr cond, Block::Ptr block);
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr cond_;
    Block::Ptr      block_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_WHILE_H
