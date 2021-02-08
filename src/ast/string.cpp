#include "ast/string.h"
#include "json.h"

using namespace std;

namespace AST {

String::String(const yy::location &loc, string value)
    : Expression(loc)
    , value{move(value)} {}

void
String::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "string");
    obj.KeyValue("value", value);
}

} // namespace AST