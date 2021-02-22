#include "ast/call.h"

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
Call::walk(const Func &fn) const {
    LValue::walk(fn);
    func_->walk(fn);
    if (arg_) {
        (*arg_)->walk(fn);
    }
}

const string &
Call::getTypeName() const {
    const static string name = "Call";
    return name;
}

} // namespace AST
