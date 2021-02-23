#include "ast/value_declaration.h"

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

ValueDeclaration::ValueDeclaration(const yy::location &loc, string variable, Expression::Ptr value)
    : Declaration(loc, move(variable))
    , value_{move(value)} {}

void
ValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "value declaration");
    obj.printKeyValue("variable", getVariable());
    obj.printKeyValue("value", value_);
}

const Expression &
ValueDeclaration::getValue() const {
    return *value_;
}

void
ValueDeclaration::walk(const function<void(const Node &)> &fn) const {
    Declaration::walk(fn);
    value_->walk(fn);
}

const string &
ValueDeclaration::getNodeName() const {
    const static string name = "Value Decl";
    return name;
}

TypeChecker::Type &
ValueDeclaration::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = value_->getType(ctx);
    assignType(ctx, type, true);
    return type;
}

} // namespace AST
