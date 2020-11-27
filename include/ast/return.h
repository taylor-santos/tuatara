#ifndef RETURN_H
#define RETURN_H

#include "statement.h"
#include "expression.h"

#include <optional>
#include <memory>

namespace AST {

class Return final : public Statement {
public: // Methods
    Return(const yy::location &loc);
    Return(const yy::location &loc, Expression::Ptr value);

private: // Fields
    std::optional<Expression::Ptr> value;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // RETURN_H
