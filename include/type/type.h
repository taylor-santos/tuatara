#ifndef TYPE_TYPE_H
#define TYPE_TYPE_H

#include <memory>
#include <vector>

#include "ast.h"
#include "location.hh"

namespace TypeChecker {

class Type : public AST::Node {
public: // Aliases
    using Ptr = std::unique_ptr<Type>;
    using Vec = std::vector<Ptr>;

public:
    explicit Type(yy::location loc);
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
