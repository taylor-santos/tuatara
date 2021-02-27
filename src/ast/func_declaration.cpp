#include "ast/func_declaration.h"

#include <algorithm>

#include "pattern/pattern.h"

#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;

} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::back_inserter;
using std::function;
using std::make_shared;
using std::make_unique;
using std::optional;
using std::ostream;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;

namespace AST {

FuncDeclaration::FuncDeclaration(
    const yy::location &                    loc,
    const yy::location &                    varLoc,
    string                                  variable,
    vector<unique_ptr<Pattern::Pattern>>    args,
    optional<shared_ptr<TypeChecker::Type>> retType)
    : Declaration(loc, varLoc, move(variable))
    , args_{move(args)}
    , retType_{move(retType)} {}

FuncDeclaration::~FuncDeclaration() = default;

void
FuncDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("args", args_);
    obj.printKeyValue("return type", retType_);
}

const vector<unique_ptr<Pattern::Pattern>> &
FuncDeclaration::getArgs() const {
    return args_;
}

const optional<shared_ptr<TypeChecker::Type>> &
FuncDeclaration::getRetType() const {
    return retType_;
}

void
FuncDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    for_each(args_.begin(), args_.end(), [&](const auto &a) { a->walk(fn); });
    if (retType_) {
        (*retType_)->walk(fn);
    }
}

const string &
FuncDeclaration::getNodeName() const {
    const static string name = "Func Decl";
    return name;
}

TypeChecker::Context &
FuncDeclaration::calculateContext(TypeChecker::Context &outerCtx) {
    implCtx_ = make_unique<TypeChecker::Context>(outerCtx);
    vector<reference_wrapper<TypeChecker::Type>> types;
    for (const auto &arg : args_) {
        auto type = arg->getType(*implCtx_);
        // TODO
    }
    return *implCtx_;
}

shared_ptr<TypeChecker::Type>
FuncDeclaration::getDeclTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
