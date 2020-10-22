#ifndef BLOCK_H
#define BLOCK_H

#include "expression.h"
#include "lvalue.h"

#include <memory>
#include <vector>

namespace AST {

class Block final : public Expression {
private: // Fields
    std::vector<std::unique_ptr<Statement>> stmts;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    Block(const yy::location &loc, std::vector<std::unique_ptr<Statement>> stmts);
};

} // namespace AST

#endif // BLOCK_H
