#include "pattern/tuple.h"

#include "json.h"

using namespace std;

namespace Pattern {

Tuple::Tuple(const yy::location &loc, Pattern::Vec patterns)
    : Pattern(loc)
    , patterns_{std::move(patterns)} {}

void
Tuple::walk(const AST::Node::Func &fn) const {
    Pattern::walk(fn);
    for (const auto &pattern : patterns_) {
        pattern->walk(fn);
    }
}

const std::string &
Tuple::getTypeName() const {
    const static string name = "Tuple Pattern";
    return name;
}
void
Tuple::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "tuple");
    obj.printKeyValue("patterns", patterns_);
}

} // namespace Pattern