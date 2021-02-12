#include "type/unit.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Unit::Unit(yy::location loc)
    : Type(loc) {}

void
Unit::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "unit");
}

void
Unit::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
}

const string &
Unit::getTypeName() const {
    const static string name = "Unit Type";
    return name;
}

} // namespace TypeChecker
