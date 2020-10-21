#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"

namespace AST {

class Statement : public Node {
protected: // Methods
    explicit Statement(const yy::location &loc);
};

} // namespace AST

#endif // STATEMENT_H
