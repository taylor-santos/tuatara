#include "ast/typed_variable.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

TypedVariable::TypedVariable(
    const yy::location &          loc,
    string                        name,
    shared_ptr<TypeChecker::Type> type)
    : Variable(loc, move(name))
    , type{move(type)} {}

void
TypedVariable::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "typed variable");
    obj.KeyValue("name", name);
    obj.Key("type");
    os << *type;
}
