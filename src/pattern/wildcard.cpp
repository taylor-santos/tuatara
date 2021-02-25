#include "pattern/wildcard.h"

#include "type/unit.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::make_unique, std::ostream, std::string;

namespace Pattern {

Wildcard::Wildcard(const yy::location &loc)
    : Pattern(loc)
    , type_{make_unique<TypeChecker::Unit>(loc)} {}

Wildcard::~Wildcard() = default;

void
Wildcard::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "wildcard");
}

const string &
Wildcard::getNodeName() const {
    const static string name = "Wildcard Pattern";
    return name;
}

TypeChecker::Type &
Wildcard::getTypeImpl(TypeChecker::Context &) {
    return *type_;
}

} // namespace Pattern
