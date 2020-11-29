#include "ast/func_impl.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

FuncImpl::FuncImpl(
    const yy::location &            loc,
    string                          variable,
    vector<pair<string, Type::Ptr>> args,
    optional<Type::Ptr>             ret_type,
    Statement::Ptr                  body)
    : FuncDeclaration(loc, move(variable), move(args), move(ret_type))
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
        obj.KeyValue("return type", **retType);
    }
}
