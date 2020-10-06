#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"

namespace TuataraAST {

class Statement : public AST {
protected:
    explicit Statement(const yy::location &loc);
};

} // namespace TuataraAST

#endif // STATEMENT_H
