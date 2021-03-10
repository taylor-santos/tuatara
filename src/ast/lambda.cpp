#include "ast/lambda.h"

#include <algorithm>
#include <cassert>

#include "pattern/pattern.h"

#include "type/func.h"
#include "type/product.h"
#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"
#include "type/unit.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::optional;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace AST {

Lambda::Lambda(
    const yy::location &                                loc,
    vector<pair<string, shared_ptr<TypeChecker::Type>>> args,
    unique_ptr<Expression>                              body)
    : Expression(loc)
    , args_{move(args)}
    , body_{move(body)} {
    assert(all_of(args_.begin(), args_.end(), [](const auto &a) -> bool { return !!a.second; }));
    assert(body_);
}

Lambda::~Lambda() = default;

void
Lambda::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "lambda");
    obj.printKey("args");
    {
        JSON::Array arr(os);
        for (const auto &[name, type] : args_) {
            arr.next();
            JSON::Object arg(os);
            arg.printKeyValue("name", name);
            arg.printKeyValue("type", type);
        }
    }
    obj.printKeyValue("body", body_);
}

void
Lambda::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    for_each(args_.begin(), args_.end(), [&](const auto &a) { a.second->walk(fn); });
    body_->walk(fn);
}

const string &
Lambda::getNodeName() const {
    const static string name = "Lambda";
    return name;
}

shared_ptr<TypeChecker::Type>
Lambda::getTypeImpl(TypeChecker::Context &ctx) {
    auto                                  newCtx = ctx;
    vector<shared_ptr<TypeChecker::Type>> argTypes;
    argTypes.reserve(args_.size());
    transform(args_.begin(), args_.end(), back_inserter(argTypes), [&](auto &arg) {
        arg.second = arg.second->verify(newCtx);
        arg.second->setInitialized(true);
        newCtx.setSymbol(arg.first, arg.second);
        return arg.second;
    });
    auto argType = argTypes.empty() ? make_shared<TypeChecker::Unit>(getLoc())
                   : argTypes.size() > 1
                       ? make_shared<TypeChecker::Product>(getLoc(), move(argTypes))
                       : move(argTypes[0]);
    auto retType = body_->getType(newCtx);
    return make_shared<TypeChecker::Func>(getLoc(), move(argType), move(retType));
}

} // namespace AST
