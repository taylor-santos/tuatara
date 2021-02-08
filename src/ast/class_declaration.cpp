#include "ast/class_declaration.h"

#include "json.h"

using namespace std;

namespace AST {

ClassDeclaration::ClassDeclaration(
    const yy::location &     loc,
    string                   name,
    std::vector<std::string> supers,
    Members                  members)
    : Declaration(loc, move(name))
    , supers{move(supers)}
    , members{move(members)} {}

void
ClassDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "class declaration");
    obj.KeyValue("name", getVariable());
    obj.Key("supers");
    {
        JSON::Array arr(os);
        for (const auto &super : supers) {
            arr.String(super);
        }
    }
    obj.Key("fields");
    {
        JSON::Array arr(os);
        for (const auto &field : members.fields) {
            arr.Next();
            JSON::Object fieldObj(os);
            fieldObj.KeyValue("name", field.name);
            fieldObj.KeyValue("type", *field.type);
        }
    }
    obj.Key("methods");
    { JSON::Array(os) << members.methods; }
    obj.Key("operators");
    {
        JSON::Array arr(os);
        for (const auto &op : members.operators) {
            arr.Next();
            JSON::Object operatorObj(os);
            operatorObj.KeyValue("operation", op.operation);
            operatorObj.Key("arg");
            {
                JSON::Object argObj(os);
                argObj.KeyValue("name", op.arg.first);
                argObj.KeyValue("type", *op.arg.second);
            }
            if (op.retType) {
                operatorObj.KeyValue("return type", **op.retType);
            }
        }
    }
    obj.Key("constructors");
    {
        JSON::Array arr(os);
        for (const auto &ctor : members.ctors) {
            arr.Next();
            JSON::Object ctorObj(os);
            ctorObj.Key("args");
            {
                JSON::Array argArr(os);
                for (const auto &arg : ctor.args) {
                    argArr.Next();
                    JSON::Object argObj(os);
                    argObj.KeyValue("name", arg.first);
                    argObj.KeyValue("type", *arg.second);
                }
            }
        }
    }
}

} // namespace AST
