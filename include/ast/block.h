#ifndef AST_BLOCK_H
#define AST_BLOCK_H

#include "ast/expression.h"
#include "ast/lvalue.h"

namespace AST {

class Block final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Block>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Block(const yy::location &loc, Expression::Vec stmts);
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Expression::Vec stmts_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_BLOCK_H
