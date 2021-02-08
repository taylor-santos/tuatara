#include "ast/func_impl.h"

#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

FuncImpl::FuncImpl(
    const yy::location &loc,
    string              variable,
    vector<Type::Named> args,
    Type::Ptr           retType,
    Statement::Ptr      body)
    : FuncDeclaration(loc, move(variable), move(args), move(retType))
    , body{move(body)} {}

FuncImpl::FuncImpl(
    const yy::location &loc,
    string              variable,
    vector<Type::Named> args,
    Statement::Ptr      body)
    : FuncDeclaration(loc, move(variable), move(args))
    , body{move(body)} {}

void
FuncImpl::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "function impl");
    obj.KeyValue("variable", getVariable());
    {
        obj.Key("args");
        JSON::Array arr(os);
        for (const auto &arg : getArgs()) {
            arr.Next();
            JSON::Object argObj(os);
            argObj.KeyValue("name", arg.first);
            argObj.KeyValue("type", *arg.second);
        }
    }
    auto retType = getRetType();
    if (retType) {
        obj.KeyValue("return type", *retType);
    }
    obj.KeyValue("body", *body);
}
