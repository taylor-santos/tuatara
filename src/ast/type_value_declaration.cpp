#include "ast/type_value_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

TypeValueDeclaration::TypeValueDeclaration(
    const yy::location &   loc,
    const string &         variable,
    shared_ptr<Type>       type,
    unique_ptr<Expression> value)
    : Declaration(loc, variable)
    , ValueDeclaration(loc, variable, move(value))
    , TypeDeclaration(loc, variable, move(type)) {}

void
TypeValueDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "type value declaration");
    obj.KeyValue("variable", variable);
    obj.Key("type");
    os << *type;
    obj.Key("value");
    os << *value;
}
