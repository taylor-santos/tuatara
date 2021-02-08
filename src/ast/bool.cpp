#include "ast/bool.h"
#include "json.h"

using namespace std;

namespace AST {

Bool::Bool(const yy::location &loc, bool value)
    : Expression(loc)
    , value{value} {}

void
Bool::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "bool");
    obj.KeyValue("value", value);
}

} // namespace AST
