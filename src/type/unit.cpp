#include "type/unit.h"

#include <common.h>

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::ostream;
using std::string;

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
Unit::isSubtype(const Type &other, Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Unit::isSuperImpl(const TypeChecker::Array &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const TypeChecker::Func &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const TypeChecker::Maybe &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const TypeChecker::Object &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const TypeChecker::Product &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const TypeChecker::Sum &, Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Unit &, Context &) const {
    return true;
}

} // namespace TypeChecker
