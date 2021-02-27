#include "ast/type_value_declaration.h"

#include "type/type.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

TypeValueDeclaration::TypeValueDeclaration(
    const yy::location &          loc,
    const yy::location &          varLoc,
    const string &                variable,
    shared_ptr<TypeChecker::Type> type,
    unique_ptr<Expression>        value)
    : Declaration(loc, varLoc, variable)
    , ValueDeclaration(loc, varLoc, variable, move(value))
    , TypeDeclaration(loc, varLoc, variable, move(type)) {}

TypeValueDeclaration::~TypeValueDeclaration() = default;

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

shared_ptr<TypeChecker::Type>
TypeValueDeclaration::getDeclTypeImpl(TypeChecker::Context &ctx) {
    auto typeType = TypeDeclaration::getDeclTypeImpl(ctx);
    assignType(typeType, ctx);
    auto valueType = ValueDeclaration::getDeclTypeImpl(ctx);
    return valueType;
}

void
TypeValueDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    getDeclType().walk(fn);
    getValue().walk(fn);
}

} // namespace AST
