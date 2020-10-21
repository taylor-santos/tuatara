#ifndef BLOCK_H
#define BLOCK_H

#include "expression.h"
#include "lvalue.h"

#include <memory>
#include <vector>

namespace AST {

class Block : public Expression {
protected:
    std::vector<std::unique_ptr<Statement>> stmts;

public:
    Block(const yy::location &loc, std::vector<std::unique_ptr<Statement>> stmts);

protected:
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // BLOCK_H
