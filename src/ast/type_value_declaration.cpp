#include "ast/type_value_declaration.h"

#include "type/type_exception.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

namespace AST {

TypeValueDeclaration::TypeValueDeclaration(
    const yy::location &loc,
    const yy::location &varLoc,
    const string &      variable,
    Type::Ptr           type,
    Expression::Ptr     value)
    : Declaration(loc, varLoc, variable)
    , ValueDeclaration(loc, varLoc, variable, move(value))
    , TypeDeclaration(loc, varLoc, variable, move(type)) {}

void
TypeValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "type value declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("type", getDeclType());
    obj.printKeyValue("value", getValue());
}

const string &
TypeValueDeclaration::getNodeName() const {
    const static string name = "Type Value Decl";
    return name;
}

Type &
TypeValueDeclaration::getTypeImpl(TypeChecker::Context &ctx) {
    TypeDeclaration::getTypeImpl(ctx);
    return ValueDeclaration::getTypeImpl(ctx);
}

void
TypeValueDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    getDeclType().walk(fn);
    getValue().walk(fn);
}

} // namespace AST
