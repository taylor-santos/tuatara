#include "ast/string.h"

#include "json.h"

using namespace std;

namespace AST {

String::String(const yy::location &loc, string value)
    : Literal(loc)
    , value_{move(value)} {}

void
String::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "string");
    obj.printKeyValue("value", value_);
}

const string &
String::getTypeName() const {
    const static string name = "String";
    return name;
}

} // namespace AST
