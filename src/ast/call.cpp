#include "ast/call.h"
#include "json.h"

using namespace std;

namespace AST {

Call::Call(const yy::location &loc, Expression::Ptr function, optional<Expression::Ptr> arg)
    : LValue(loc)
    , function{move(function)}
    , arg{move(arg)} {}

void
Call::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "function call");
    obj.KeyValue("function", *function);
    if (arg) {
        obj.KeyValue("arg", **arg);
    }
}

} // namespace AST