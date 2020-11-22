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
    If(const yy::location &loc, std::unique_ptr<Expression> cond, std::unique_ptr<Statement> stmt);
    If(const yy::location &        loc,
       std::unique_ptr<Expression> cond,
       std::unique_ptr<Statement>  stmt,
       std::unique_ptr<Statement>  else_stmt);

private: // Fields
    std::unique_ptr<Expression>               cond;
    std::unique_ptr<Statement>                stmt;
    std::optional<std::unique_ptr<Statement>> else_stmt;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // IF_H
