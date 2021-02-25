#include "ast/literal.h"

#include "type/object.h"

namespace yy {
class location;
} // namespace yy

using std::make_unique, std::string;

namespace AST {

Literal::Literal(const yy::location &loc, string className)
    : Expression(loc)
    , myType_{make_unique<TypeChecker::Object>(loc, move(className))} {}

Literal::~Literal() = default;

TypeChecker::Object &
Literal::getMyType() {
    return *myType_;
}

} // namespace AST
