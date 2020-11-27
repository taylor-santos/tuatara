#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"

#include <memory>
#include <vector>

namespace AST {

class Statement : public Node {
public:
    using Ptr = std::unique_ptr<Statement>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    explicit Statement(const yy::location &loc);
};

} // namespace AST

#endif // STATEMENT_H
