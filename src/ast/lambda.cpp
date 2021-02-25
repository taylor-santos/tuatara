#include "ast/lambda.h"

#include <algorithm>

#include "pattern/pattern.h"

#include "type/type.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function, std::optional, std::ostream, std::string, std::unique_ptr, std::vector;

namespace AST {

Lambda::Lambda(
    const yy::location &                    loc,
    vector<unique_ptr<Pattern::Pattern>>    args,
    optional<unique_ptr<TypeChecker::Type>> retType,
    unique_ptr<Expression>                  body)
    : Expression(loc)
    , args_{move(args)}
    , retType_{move(retType)}
    , body_{move(body)} {}

Lambda::~Lambda() = default;

void
Lambda::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "lambda");
    obj.printKeyValue("args", args_);
    obj.printKeyValue("return type", retType_);
    obj.printKeyValue("body", body_);
}

void
Lambda::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    for_each(args_.begin(), args_.end(), [&](const auto &a) { a->walk(fn); });
    if (retType_) {
        (*retType_)->walk(fn);
    }
    body_->walk(fn);
}

const string &
Lambda::getNodeName() const {
    const static string name = "Lambda";
    return name;
}

TypeChecker::Type &
Lambda::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
