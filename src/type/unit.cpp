#include "type/unit.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::make_shared;
using std::ostream;
using std::string;

namespace TypeChecker {

Unit::Unit(yy::location loc)
    : Type(loc) {}

Unit::~Unit() = default;

std::shared_ptr<Type>
Unit::clone(const yy::location &loc) const {
    return make_shared<Unit>(loc);
}

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
Unit::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Unit::isSuperImpl(const Array &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Func &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Maybe &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Object &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Product &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Sum &, const Context &) const {
    return true;
}

bool
Unit::isSuperImpl(const Unit &, const Context &) const {
    return true;
}

void
Unit::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

} // namespace TypeChecker
