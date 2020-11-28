#ifndef AST_STATEMENT_H
#define AST_STATEMENT_H

#include "ast.h"

namespace AST {

class Statement : public Node {
public: // Aliases
    using Ptr = std::unique_ptr<Statement>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit Statement(const yy::location &loc);
};

} // namespace AST

#endif // AST_STATEMENT_H
