#include "ast/declaration.h"

#include <sstream>

#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"

using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

namespace AST {

Declaration::Declaration(const yy::location &loc, const yy::location &varLoc, string variable)
    : Expression(loc)
    , varLoc_{varLoc}
    , variable_{move(variable)} {}

Declaration::~Declaration() = default;

const string &
Declaration::getVariable() const {
    return variable_;
}

std::shared_ptr<TypeChecker::Type>
Declaration::assignType(const std::shared_ptr<TypeChecker::Type> &type, TypeChecker::Context &ctx) {
    ctx.updateSymbol(variable_, type);
    return type;
}

std::shared_ptr<TypeChecker::Type>
Declaration::getTypeImpl(TypeChecker::Context &ctx) {
    auto type = getDeclTypeImpl(ctx);
    assignType(type, ctx);
    return type;
}

} // namespace AST
