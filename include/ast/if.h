#ifndef AST_IF_H
#define AST_IF_H

#include "ast/block.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {
class Node;

class If : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<If>;
    using Vec = std::vector<Ptr>;

public: // Methods
    If(const yy::location &loc, Expression::Ptr cond, Block::Ptr block);
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getCond() const;
    [[nodiscard]] const Expression &getBlock() const;

private: // Fields
    Expression::Ptr cond_;
    Block::Ptr      block_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_IF_H
