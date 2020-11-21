#ifndef RETURN_H
#define RETURN_H

#include "statement.h"
#include "expression.h"

#include <optional>
#include <memory>

namespace AST {

class Return final : public Statement {
private: // Fields
    std::optional<std::unique_ptr<Expression>> value;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    Return(const yy::location &loc);
    Return(const yy::location &loc, std::unique_ptr<Expression> value);
};

} // namespace AST

#endif // RETURN_H
