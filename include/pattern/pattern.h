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
public: // Methods
    explicit Pattern(const yy::location &loc);
    ~Pattern() override;
    std::shared_ptr<TypeChecker::Type> getType(TypeChecker::Context &ctx);
    virtual const std::string *        getName();

private: // Methods
    virtual std::shared_ptr<TypeChecker::Type> getTypeImpl(TypeChecker::Context &ctx) = 0;
};

} // namespace Pattern

#endif // PATTERN_PATTERN_H
