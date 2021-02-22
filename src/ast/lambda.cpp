#include "ast/lambda.h"

#include "json.h"

using namespace std;

namespace AST {

Lambda::Lambda(
    const yy::location &                  loc,
    Pattern::Pattern::Vec                 args,
    std::optional<TypeChecker::Type::Ptr> retType,
    Expression::Ptr                       body)
    : Expression(loc)
    , args_{move(args)}
    , retType_{move(retType)}
    , body_{move(body)} {}

void
Lambda::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "lambda");
    obj.printKeyValue("args", args_);
    obj.printKeyValue("return type", retType_);
    obj.printKeyValue("body", body_);
}

void
Lambda::walk(const Func &fn) const {
    Expression::walk(fn);
    for (const auto &arg : args_) {
        arg->walk(fn);
    }
    if (retType_) {
        (*retType_)->walk(fn);
    }
    body_->walk(fn);
}

const string &
Lambda::getTypeName() const {
    const static string name = "Lambda";
    return name;
}

} // namespace AST
