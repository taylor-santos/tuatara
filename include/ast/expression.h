#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "statement.h"

namespace AST {

class Expression : public Statement {
protected: // Methods
    explicit Expression(const yy::location &loc);
};

} // namespace AST

#endif // EXPRESSION_H
