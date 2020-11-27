#ifndef STATEMENT_H
#define STATEMENT_H

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

#endif // STATEMENT_H
