#include "pattern/literal.h"

#include "json.h"

using namespace std;

namespace Pattern {

Literal::Literal(const yy::location &loc, AST::Literal::Ptr literal)
    : Pattern(loc)
    , literal_{move(literal)} {}

void
Literal::walk(const AST::Node::Func &fn) const {
    Pattern::walk(fn);
    literal_->walk(fn);
}

const std::string &
Literal::getTypeName() const {
    const static string name = "Literal Pattern";
    return name;
}
void
Literal::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "literal");
    obj.printKeyValue("literal", literal_);
}

} // namespace Pattern