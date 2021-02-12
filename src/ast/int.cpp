#include "ast/int.h"

#include "json.h"

using namespace std;

namespace AST {

Int::Int(const yy::location &loc, int64_t value)
    : Literal(loc)
    , value_{value} {}

void
Int::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "int");
    obj.printKeyValue("value", value_);
}

const string &
Int::getTypeName() const {
    const static string name = "Int";
    return name;
}

} // namespace AST
