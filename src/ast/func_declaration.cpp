#include "ast/func_declaration.h"

#include <algorithm>

#include "pattern/pattern.h"

#include "type/type.h"
#include "type/type_exception.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

FuncDeclaration::FuncDeclaration(
    const yy::location &  loc,
    const yy::location &  varLoc,
    string                variable,
    Pattern::Pattern::Vec args,
    optional<Type::Ptr>   retType)
    : Declaration(loc, varLoc, move(variable))
    , args_{move(args)}
    , retType_{move(retType)} {}

void
FuncDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "function declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("args", args_);
    obj.printKeyValue("return type", retType_);
}

const Pattern::Pattern::Vec &
FuncDeclaration::getArgs() const {
    return args_;
}

const std::optional<TypeChecker::Type::Ptr> &
FuncDeclaration::getRetType() const {
    return retType_;
}

const string &
FuncDeclaration::getNodeName() const {
    const static string name = "Func Decl";
    return name;
}

Type &
FuncDeclaration::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

void
FuncDeclaration::walk(const std::function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    for_each(args_.begin(), args_.end(), [&](const auto &a) { a->walk(fn); });
    if (retType_) {
        (*retType_)->walk(fn);
    }
}

} // namespace AST
