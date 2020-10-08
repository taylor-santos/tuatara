#include "ast/type_declaration.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace std;

TypeDeclaration::TypeDeclaration(const yy::location &loc, string variable)
    : Declaration(loc, move(variable)) {}

void
TypeDeclaration::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "type declaration");
    obj.KeyValue("variable", variable);
}
