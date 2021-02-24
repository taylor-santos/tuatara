#include "ast/expression.h"

#include "type/type.h"
#include "type/type_exception.h"

namespace AST {

Expression::Expression(const yy::location &loc)
    : Node(loc)
    , myType_{nullptr} {}

TypeChecker::Type &
Expression::getType(TypeChecker::Context &ctx) {
    if (myType_) {
        return *myType_;
    }
    myType_ = &getTypeImpl(ctx);
    myType_->verify(ctx);
    return *myType_;
}

} // namespace AST
