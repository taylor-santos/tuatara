#include "pattern/named_wildcard.h"

#include "json.h"

using namespace std;

namespace Pattern {

NamedWildcard::NamedWildcard(const yy::location &loc, std::string name)
    : Wildcard(loc)
    , name_{std::move(name)} {}

const std::string &
NamedWildcard::getTypeName() const {
    const static string name = "Named Wildcard Pattern";
    return name;
}
void
NamedWildcard::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named wildcard");
    obj.printKeyValue("name", name_);
}

} // namespace Pattern