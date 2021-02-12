#include "ast/float.h"

#include "json.h"

using namespace std;

namespace AST {

Float::Float(const yy::location &loc, double value)
    : Literal(loc)
    , value_{value} {}

void
Float::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "float");
    obj.printKeyValue("value", value_);
}

const string &
Float::getTypeName() const {
    const static string name = "Float";
    return name;
}

} // namespace AST
