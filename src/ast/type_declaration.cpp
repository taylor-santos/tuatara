#include "ast/type_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace TypeChecker;
using namespace std;

TypeDeclaration::TypeDeclaration(const yy::location &loc, string variable, Type::Ptr type)
    : Declaration(loc, move(variable))
    , type{move(type)} {}

void
TypeDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "type declaration");
    obj.KeyValue("variable", getVariable());
    obj.Key("type");
    os << *type;
}

const TypeChecker::Type::Ptr &
TypeDeclaration::getType() const {
    return type;
}
