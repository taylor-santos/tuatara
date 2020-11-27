#ifndef BLOCK_H
#define BLOCK_H

#include "expression.h"
#include "lvalue.h"

#include <memory>
#include <vector>

namespace AST {

class Block final : public Expression {
public: // Methods
    Block(const yy::location &loc, Statement::Vec stmts);

private: // Fields
    Statement::Vec stmts;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // BLOCK_H
