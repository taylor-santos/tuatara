#include "ast/value_declaration.h"
#include "json.h"

using namespace std;

namespace AST {

ValueDeclaration::ValueDeclaration(const yy::location &loc, string variable, Expression::Ptr value)
    : Declaration(loc, move(variable))
    , value{move(value)} {}

void
ValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "value declaration");
    obj.KeyValue("variable", getVariable());
    obj.Key("value");
    os << *value;
}

const Expression::Ptr &
ValueDeclaration::getValue() const {
    return value;
}

} // namespace AST