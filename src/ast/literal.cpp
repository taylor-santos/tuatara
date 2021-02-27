#include "ast/literal.h"

#include <utility>

#include "type/object.h"

namespace yy {
class location;
} // namespace yy

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::string;

namespace AST {

Literal::Literal(const yy::location &loc, string className)
    : Expression(loc)
    , className_{std::move(className)} {}

Literal::~Literal() = default;

shared_ptr<TypeChecker::Type>
Literal::getTypeImpl(TypeChecker::Context &ctx) {
    auto type = make_shared<TypeChecker::Object>(getLoc(), className_);
    type->verify(ctx);
    type->setInitialized(true);
    return type;
}

} // namespace AST
