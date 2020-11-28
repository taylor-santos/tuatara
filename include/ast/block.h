#ifndef AST_BLOCK_H
#define AST_BLOCK_H

#include "expression.h"
#include "lvalue.h"

namespace AST {

class Block final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Block>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Block(const yy::location &loc, Statement::Vec stmts);

private: // Fields
    Statement::Vec stmts;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_BLOCK_H
