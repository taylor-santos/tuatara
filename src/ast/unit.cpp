#include "ast/unit.h"

#include "type/type_exception.h"
#include "type/unit.h"

#include "json.h"

using namespace std;

namespace AST {

Unit::Unit(const yy::location &loc)
    : LValue(loc)
    , myType_(loc) {}

void
Unit::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "unit");
}

const string &
Unit::getNodeName() const {
    const static string name = "Unit";
    return name;
}

TypeChecker::Type &
Unit::getTypeImpl(TypeChecker::Context &) {
    return myType_;
}

} // namespace AST
