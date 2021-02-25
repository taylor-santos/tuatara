#include "pattern/wildcard.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace Pattern {

Wildcard::Wildcard(const yy::location &loc)
    : Pattern(loc)
    , type_{loc} {}

void
Wildcard::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "wildcard");
}

const std::string &
Wildcard::getNodeName() const {
    const static string name = "Wildcard Pattern";
    return name;
}

TypeChecker::Type &
Wildcard::getTypeImpl(TypeChecker::Context &ctx) {
    return type_;
}

} // namespace Pattern
