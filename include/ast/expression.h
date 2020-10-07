#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "statement.h"

namespace TuataraAST {

class Expression : public Statement {
protected:
    explicit Expression(const yy::location &loc);
};

} // namespace TuataraAST

#endif // EXPRESSION_H
