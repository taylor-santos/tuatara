#ifndef WHILE_H
#define WHILE_H

#include <vector>
#include <memory>
#include <optional>

#include "statement.h"
#include "expression.h"

namespace AST {

class While final : public Statement {
public: // Methods
    While(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt);

private: // Fields
    Expression::Ptr cond;
    Statement::Ptr  stmt;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // WHILE_H