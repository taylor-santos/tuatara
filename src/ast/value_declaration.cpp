#include "ast/value_declaration.h"

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
ValueDeclaration::walk(const Func &fn) const {
    Declaration::walk(fn);
    value_->walk(fn);
}

const string &
ValueDeclaration::getTypeName() const {
    const static string name = "Value Decl";
    return name;
}

} // namespace AST
