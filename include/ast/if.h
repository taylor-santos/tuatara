#ifndef AST_IF_H
#define AST_IF_H

#include "statement.h"
#include "expression.h"

#include <optional>

namespace AST {

class If final : public Statement {
public: // Aliases
    using Ptr = std::unique_ptr<If>;
    using Vec = std::vector<Ptr>;

public: // Methods
    If(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt);
    If(const yy::location &loc,
       Expression::Ptr     cond,
       Statement::Ptr      stmt,
       Statement::Ptr      else_stmt);

private: // Fields
    Expression::Ptr               cond;
    Statement::Ptr                stmt;
    std::optional<Statement::Ptr> else_stmt;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_IF_H
