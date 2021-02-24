#include "pattern/literal.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace Pattern {

Literal::Literal(const yy::location &loc, AST::Literal::Ptr literal)
    : Pattern(loc)
    , literal_{move(literal)} {}

void
Literal::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "literal");
    obj.printKeyValue("literal", literal_);
}

void
Literal::walk(const std::function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    literal_->walk(fn);
}

const std::string &
Literal::getNodeName() const {
    const static string name = "Literal Pattern";
    return name;
}

} // namespace Pattern
