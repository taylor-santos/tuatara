#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "ast/expression.h"

namespace AST {

class Literal : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Literal>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Literal(const yy::location &loc);
    ~Literal() override = default;
};

} // namespace AST

#endif // AST_LITERAL_H
