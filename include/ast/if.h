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

class If : public Expression {
public: // Methods
    If(const yy::location &loc, std::unique_ptr<Expression> cond, std::unique_ptr<Block> block);
    ~If() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getCond() const;
    [[nodiscard]] const Expression &getBlock() const;

private: // Fields
    std::unique_ptr<Expression> cond_;
    std::unique_ptr<Block>      block_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_IF_H
