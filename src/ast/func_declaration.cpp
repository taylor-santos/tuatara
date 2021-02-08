#include "ast/func_declaration.h"

#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

FuncDeclaration::FuncDeclaration(
    const yy::location &loc,
    string              variable,
    vector<Type::Named> args,
    Type::Ptr           retType)
    : Declaration(loc, move(variable))
    , args{move(args)}
    , retType{move(retType)} {}

FuncDeclaration::FuncDeclaration(const yy::location &loc, string variable, vector<Type::Named> args)
    : Declaration(loc, move(variable))
    , args{move(args)} {}

void
FuncDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "function declaration");
    obj.KeyValue("variable", getVariable());
    {
        obj.Key("args");
        JSON::Array arr(os);
        for (const auto &arg : args) {
            arr.Next();
            JSON::Object argObj(os);
            argObj.KeyValue("name", arg.first);
            argObj.KeyValue("type", *arg.second);
        }
    }
    if (retType) {
        obj.KeyValue("return type", *retType);
    }
}

const vector<TypeChecker::Type::Named> &
FuncDeclaration::getArgs() const {
    return args;
}

optional<reference_wrapper<const TypeChecker::Type>>
FuncDeclaration::getRetType() const {
    if (retType) {
        return *retType;
    }
    return {};
}
