#include "pattern/named_wildcard.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::ostream, std::string;

namespace Pattern {

NamedWildcard::NamedWildcard(const yy::location &loc, string name)
    : Wildcard(loc)
    , name_{move(name)} {}

NamedWildcard::~NamedWildcard() = default;

void
NamedWildcard::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named wildcard");
    obj.printKeyValue("name", name_);
}

const string &
NamedWildcard::getNodeName() const {
    const static string name = "Named Wildcard Pattern";
    return name;
}

const string *
NamedWildcard::getName() {
    return &name_;
}

} // namespace Pattern
