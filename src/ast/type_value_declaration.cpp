#include "ast/type_value_declaration.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

TypeValueDeclaration::TypeValueDeclaration(
    const yy::location &loc,
    const string &      variable,
    Type::Ptr           type,
    Expression::Ptr     value)
    : Declaration(loc, variable)
    , ValueDeclaration(loc, variable, move(value))
    , TypeDeclaration(loc, variable, move(type)) {}

void
TypeValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "type value declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("type", getType());
    obj.printKeyValue("value", getValue());
}

const string &
TypeValueDeclaration::getTypeName() const {
    const static string name = "Type Value Decl";
    return name;
}

void
TypeValueDeclaration::walk(const Func &fn) const {
    Declaration::walk(fn);
    getType().walk(fn);
    getValue().walk(fn);
}

} // namespace AST
