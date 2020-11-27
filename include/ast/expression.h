#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "statement.h"

namespace AST {

class Expression : public Statement {
public:
    using Ptr = std::unique_ptr<Expression>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit Expression(const yy::location &loc);
};

} // namespace AST

#endif // EXPRESSION_H
