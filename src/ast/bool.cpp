#include "ast/bool.h"

#include "json.h"

using namespace std;

namespace AST {

Bool::Bool(const yy::location &loc, bool value)
    : Literal(loc)
    , value_{value} {}

void
Bool::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "bool");
    obj.printKeyValue("value", value_);
}

const string &
Bool::getTypeName() const {
    const static string name = "Bool";
    return name;
}

} // namespace AST
