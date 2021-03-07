#include "ast/expression.h"

#include "type/type.h"

namespace yy {
class location;
} // namespace yy
using std::make_shared;
using std::shared_ptr;

namespace AST {

Expression::Expression(const yy::location &loc)
    : Node(loc) {}

Expression::~Expression() = default;

shared_ptr<TypeChecker::Type>
Expression::getType(TypeChecker::Context &ctx) {
    auto type = getTypeImpl(ctx);
    type      = TypeChecker::Type::verify(type, ctx);
    return type;
}

} // namespace AST
