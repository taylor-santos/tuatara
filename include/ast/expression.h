#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include "ast.h"

namespace AST {

class Expression : public Node {
public: // Aliases
    using Ptr = std::unique_ptr<Expression>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit Expression(const yy::location &loc);
};

} // namespace AST

#endif // AST_EXPRESSION_H
