#include "ast/call.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Call::Call(const yy::location &loc, Expression::Ptr func, Expression::Ptr arg)
    : LValue(loc)
    , func_{move(func)}
    , arg_{move(arg)} {}

void
Call::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function call");
    obj.printKeyValue("function", func_);
    obj.printKeyValue("arg", arg_);
}

void
Call::walk(const function<void(const Node &)> &fn) const {
    LValue::walk(fn);
    func_->walk(fn);
    arg_->walk(fn);
}

const string &
Call::getNodeName() const {
    const static string name = "Call";
    return name;
}

TypeChecker::Type &
Call::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = func_->getType(ctx);
    auto &ret  = type.callAsFunc(ctx, *arg_);
    return ret;
}

} // namespace AST
