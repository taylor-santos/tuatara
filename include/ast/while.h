#ifndef AST_WHILE_H
#define AST_WHILE_H

#include "statement.h"
#include "expression.h"

#include <optional>

namespace AST {

class While final : public Statement {
public: // Aliases
    using Ptr = std::unique_ptr<While>;
    using Vec = std::vector<Ptr>;

public: // Methods
    While(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt);

private: // Fields
    Expression::Ptr cond;
    Statement::Ptr  stmt;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_WHILE_H
