#ifndef RETURN_H
#define RETURN_H

#include "statement.h"
#include "expression.h"

#include <optional>

namespace AST {

class Return final : public Statement {
public: // Aliases
    using Ptr = std::unique_ptr<Return>;
    using Vec = std::vector<Ptr>;

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
