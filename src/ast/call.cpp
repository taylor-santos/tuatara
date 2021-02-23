#include "ast/call.h"

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

Call::Call(const yy::location &loc, Expression::Ptr func, optional<Expression::Ptr> arg)
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
    if (arg_) {
        (*arg_)->walk(fn);
    }
}

const string &
Call::getNodeName() const {
    const static string name = "Call";
    return name;
}

TypeChecker::Type &
Call::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException("type error: call type checking not implemented", getLoc());
}

} // namespace AST
