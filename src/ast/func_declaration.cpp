#include "ast/func_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

FuncDeclaration::FuncDeclaration(
    const yy::location &                   loc,
    string                                 variable,
    vector<pair<string, shared_ptr<Type>>> args,
    optional<shared_ptr<Type>>             ret_type,
    unique_ptr<Statement>                  body)
    : Declaration(loc, move(variable))
    , args{move(args)}
    , ret_type{move(ret_type)}
    , body{move(body)} {}

void
FuncDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "function declaration");
    obj.KeyValue("variable", variable);
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
    obj.KeyValue("body", *body);
}
