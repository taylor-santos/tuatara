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
    While(
        const yy::location &        loc,
        std::unique_ptr<Expression> cond,
        std::unique_ptr<Statement>  stmt);

private: // Fields
    std::unique_ptr<Expression> cond;
    std::unique_ptr<Statement>  stmt;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // WHILE_H