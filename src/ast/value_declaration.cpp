#include "ast/value_declaration.h"
#include "json.h"

using namespace TuataraAST;
using namespace std;

ValueDeclaration::ValueDeclaration(
    const yy::location &   loc,
    string                 variable,
    unique_ptr<Expression> value)
    : Declaration(loc, move(variable))
    , value{move(value)} {}

void
ValueDeclaration::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "value declaration");
    obj.KeyValue("variable", variable);
    obj.Key("value");
    os << *value;
}
