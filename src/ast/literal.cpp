#include "ast/literal.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Literal::Literal(const yy::location &loc, string className)
    : Expression(loc)
    , myType_(loc, std::move(className)) {}

TypeChecker::Object &
Literal::getMyType() {
    return myType_;
}

} // namespace AST
