#include "ast/call.h"

#include "type/type.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

Call::Call(const yy::location &loc, unique_ptr<Expression> func, unique_ptr<Expression> arg)
    : LValue(loc)
    , func_{move(func)}
    , arg_{move(arg)} {}

Call::~Call() = default;

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

shared_ptr<TypeChecker::Type>
Call::getTypeImpl(TypeChecker::Context &ctx) {
    auto type = func_->getType(ctx);
    auto ret  = type->callAsFunc(ctx, *arg_, *this);
    return ret;
}

} // namespace AST
