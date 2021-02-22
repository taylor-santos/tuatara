#include "ast/unit.h"

#include "json.h"

using namespace std;

namespace AST {

Unit::Unit(const yy::location &loc)
    : LValue(loc) {}

void
Unit::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "unit");
}

const string &
Unit::getTypeName() const {
    const static string name = "Unit";
    return name;
}

} // namespace AST
