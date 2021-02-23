#include "pattern/named_wildcard.h"

#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace Pattern {

NamedWildcard::NamedWildcard(const yy::location &loc, std::string name)
    : Wildcard(loc)
    , name_{std::move(name)} {}

void
NamedWildcard::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named wildcard");
    obj.printKeyValue("name", name_);
}

const std::string &
NamedWildcard::getNodeName() const {
    const static string name = "Named Wildcard Pattern";
    return name;
}

} // namespace Pattern