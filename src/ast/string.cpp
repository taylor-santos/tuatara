#include "ast/string.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

String::String(const yy::location &loc, string value)
    : Expression(loc)
    , value{move(value)} {}

void
String::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "string");
    obj.KeyValue("value", value);
}
