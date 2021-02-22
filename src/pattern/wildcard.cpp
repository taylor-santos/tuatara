#include "pattern/wildcard.h"

#include "json.h"

using namespace std;

namespace Pattern {

Wildcard::Wildcard(const yy::location &loc)
    : Pattern(loc) {}

const std::string &
Wildcard::getTypeName() const {
    const static string name = "Wildcard Pattern";
    return name;
}
void
Wildcard::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "wildcard");
}

} // namespace Pattern