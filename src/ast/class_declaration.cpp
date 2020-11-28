#include "ast/class_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace std;

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
    {
        JSON::Array arr(os);
        for (const auto &method : members.methods) {
            arr.Next();
            JSON::Object methodObj(os);
            methodObj.KeyValue("name", method.name);
            methodObj.Key("args");
            {
                JSON::Array argArr(os);
                for (const auto &arg : method.args) {
                    argArr.Next();
                    JSON::Object argObj(os);
                    argObj.KeyValue("name", arg.name);
                    argObj.KeyValue("type", *arg.type);
                }
            }
            if (method.ret_type) {
                methodObj.KeyValue("return type", **method.ret_type);
            }
        }
    }
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
                argObj.KeyValue("name", op.arg.name);
                argObj.KeyValue("type", *op.arg.type);
            }
            if (op.ret_type) {
                operatorObj.KeyValue("return type", **op.ret_type);
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
                    argObj.KeyValue("name", arg.name);
                    argObj.KeyValue("type", *arg.type);
                }
            }
        }
    }
}
