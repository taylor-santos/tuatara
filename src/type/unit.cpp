#include "type/unit.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::ostream, std::string;

namespace TypeChecker {

Unit::Unit(yy::location loc)
    : Type(loc) {}

Unit::~Unit() = default;

void
Unit::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "unit");
}

const string &
Unit::getNodeName() const {
    const static string name = "Unit Type";
    return name;
}

void
Unit::verifyImpl(Context &) {}

void
Unit::pretty(ostream &out, bool) const {
    out << "()";
}

bool
Unit::isSubtype(const Type &other) const {
    return other.isSupertype(*this);
}

bool
Unit::isSupertype(const Type &) const {
    return true;
}

} // namespace TypeChecker
