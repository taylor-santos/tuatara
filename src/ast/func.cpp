#include "ast/func.h"

#include <sstream>

#include "ast/block.h"

#include "pattern/pattern.h"

#include "type/func.h"
#include "type/product.h"
#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"
#include "type/unit.h"

#include "json.h"

using std::function;
using std::make_shared;
using std::make_unique;
using std::optional;
using std::ostream;
using std::pair;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace AST {

Func::Func(
    const yy::location &                    loc,
    const yy::location &                    varLoc,
    string                                  variable,
    vector<unique_ptr<Pattern::Pattern>>    args,
    unique_ptr<Block>                       body,
    optional<shared_ptr<TypeChecker::Type>> retType)
    : Declaration(loc, varLoc, move(variable))
    , args_{move(args)}
    , retType_{move(retType)}
    , body_{move(body)} {}

Func::~Func() = default;

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("args", args_);
    obj.printKeyValue("return type", retType_);
    obj.printKeyValue("body", body_);
}

void
Func::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    for_each(args_.begin(), args_.end(), [&](const auto &a) { a->walk(fn); });
    if (retType_) {
        (*retType_)->walk(fn);
    }
    body_->walk(fn);
}

const string &
Func::getNodeName() const {
    const static string name = "Func Impl";
    return name;
}

shared_ptr<TypeChecker::Type>
Func::getDeclTypeImpl(TypeChecker::Context &ctx) {
    auto                                  newCtx = ctx;
    vector<shared_ptr<TypeChecker::Type>> argTypes;
    argTypes.reserve(args_.size());
    transform(args_.begin(), args_.end(), back_inserter(argTypes), [&](auto &arg) {
        return arg->getType(newCtx);
    });
    // TODO: get location of arguments only
    auto argType = argTypes.empty()      ? make_shared<TypeChecker::Unit>(getLoc())
                   : argTypes.size() > 1 ? make_shared<TypeChecker::Product>(getLoc(), argTypes)
                                         : argTypes[0];
    auto bodyRet = body_->getType(newCtx);
    if (retType_) {
        (*retType_)->verify(ctx);
        if (!bodyRet->isSubtype(**retType_, ctx)) {
            vector<pair<string, yy::location>> msgs;
            stringstream                       ss;
            ss << "error: returning \"";
            bodyRet->pretty(ss);
            ss << "\" from a function expecting to return \"";
            (*retType_)->pretty(ss);
            ss << "\"";
            msgs.emplace_back(ss.str(), bodyRet->getLoc());
            throw TypeChecker::TypeException(msgs);
        }
    }
    // TODO: Type should point to the loc of the signature only
    auto type = make_shared<TypeChecker::Func>(getLoc(), argType, bodyRet);
    type->setInitialized(true);
    return type;
}

} // namespace AST
