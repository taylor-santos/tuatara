#include "ast/func_impl.h"

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
using std::optional;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace AST {

FuncImpl::FuncImpl(
    const yy::location &                    loc,
    const yy::location &                    varLoc,
    string                                  variable,
    vector<unique_ptr<Pattern::Pattern>>    args,
    unique_ptr<Block>                       body,
    optional<shared_ptr<TypeChecker::Type>> retType)
    : FuncDeclaration(loc, varLoc, move(variable), move(args), move(retType))
    , body_{move(body)} {}

FuncImpl::~FuncImpl() = default;

void
FuncImpl::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function impl");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("args", getArgs());
    obj.printKeyValue("return type", getRetType());
    obj.printKeyValue("body", body_);
}

void
FuncImpl::walk(const function<void(const Node &)> &fn) const {
    FuncDeclaration::walk(fn);
    body_->walk(fn);
}

const string &
FuncImpl::getNodeName() const {
    const static string name = "Func Impl";
    return name;
}

shared_ptr<TypeChecker::Type>
FuncImpl::getDeclTypeImpl(TypeChecker::Context &ctx) {
    auto                                  newCtx = ctx;
    auto &                                args   = getArgs();
    vector<shared_ptr<TypeChecker::Type>> argTypes;
    argTypes.reserve(args.size());
    transform(args.begin(), args.end(), back_inserter(argTypes), [&](auto &arg) {
        return arg->getType(newCtx);
    });
    // TODO: get location of arguments only
    auto argType     = argTypes.empty()      ? make_shared<TypeChecker::Unit>(getLoc())
                       : argTypes.size() > 1 ? make_shared<TypeChecker::Product>(getLoc(), argTypes)
                                             : argTypes[0];
    auto retType     = body_->getType(newCtx);
    auto explicitRet = getRetType();
    if (explicitRet) {
        (*explicitRet)->verify(ctx);
        if (!retType->isSubtype(**explicitRet, ctx)) {
            vector<pair<string, yy::location>> msgs;
            stringstream                       ss;
            ss << "error: returning \"";
            retType->pretty(ss);
            ss << "\" from a function expecting to return \"";
            (*explicitRet)->pretty(ss);
            ss << "\"";
            msgs.emplace_back(ss.str(), retType->getLoc());
            throw TypeChecker::TypeException(msgs);
        }
    }
    // TODO: Type should point to the loc of the signature only
    return make_shared<TypeChecker::Func>(getLoc(), argType, retType);
}

} // namespace AST
