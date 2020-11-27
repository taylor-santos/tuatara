#ifndef IF_H
#define IF_H

#include <vector>
#include <memory>
#include <optional>

#include "statement.h"
#include "expression.h"

namespace AST {

class If final : public Statement {
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

#endif // IF_H
