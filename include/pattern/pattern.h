#ifndef PATTERN_PATTERN_H
#define PATTERN_PATTERN_H

#include "ast.h"

namespace yy {
class location;
} // namespace yy
namespace TypeChecker {
class Type;
class Context;
} // namespace TypeChecker

namespace Pattern {

class Pattern : public AST::Node {
public: // Aliases
    using Ptr = std::unique_ptr<Pattern>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Pattern(const yy::location &loc);
    TypeChecker::Type &        getType(TypeChecker::Context &ctx);
    virtual const std::string *getName();

private: // Methods
    virtual TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) = 0;
};

} // namespace Pattern

#endif // PATTERN_PATTERN_H
