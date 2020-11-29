#include "ast/func_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

FuncDeclaration::FuncDeclaration(
    const yy::location &            loc,
    string                          variable,
    vector<pair<string, Type::Ptr>> args,
    optional<Type::Ptr>             ret_type)
    : Declaration(loc, move(variable))
    , args{move(args)}
    , ret_type{move(ret_type)} {}

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
    if (ret_type) {
        obj.KeyValue("return type", **ret_type);
    }
}

const vector<pair<string, TypeChecker::Type::Ptr>> &
FuncDeclaration::getArgs() const {
    return args;
}

const optional<TypeChecker::Type::Ptr> &
FuncDeclaration::getRetType() const {
    return ret_type;
}
