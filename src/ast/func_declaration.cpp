#include "ast/func_declaration.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

FuncDeclaration::FuncDeclaration(
    const yy::location &  loc,
    string                variable,
    Pattern::Pattern::Vec args,
    optional<Type::Ptr>   retType)
    : Declaration(loc, move(variable))
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
FuncDeclaration::getTypeName() const {
    const static string name = "Func Decl";
    return name;
}

void
FuncDeclaration::walk(const Node::Func &fn) const {
    Declaration::walk(fn);
    for (const auto &arg : args_) {
        arg->walk(fn);
    }
    if (retType_) {
        (*retType_)->walk(fn);
    }
}

} // namespace AST
