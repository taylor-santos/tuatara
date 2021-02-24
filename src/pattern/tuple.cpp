#include "pattern/tuple.h"

#include <algorithm>

#include "json.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace Pattern {

Tuple::Tuple(const yy::location &loc, Pattern::Vec patterns)
    : Pattern(loc)
    , patterns_{std::move(patterns)} {}

void
Tuple::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "tuple");
    obj.printKeyValue("patterns", patterns_);
}

void
Tuple::walk(const std::function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    for_each(patterns_.begin(), patterns_.end(), [&](const auto &p) { p->walk(fn); });
}

const std::string &
Tuple::getNodeName() const {
    const static string name = "Tuple Pattern";
    return name;
}

} // namespace Pattern
