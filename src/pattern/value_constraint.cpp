#include "pattern/value_constraint.h"

#include "json.h"

using namespace std;

namespace Pattern {

ValueConstraint::ValueConstraint(const yy::location &loc, AST::Expression::Ptr value)
    : Constraint(loc)
    , value_{move(value)} {}

void
ValueConstraint::walk(const AST::Node::Func &fn) const {
    Constraint::walk(fn);
    value_->walk(fn);
}

const std::string &
ValueConstraint::getTypeName() const {
    const static string name = "Value Constraint Pattern";
    return name;
}
void
ValueConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "value constraint");
    obj.printKeyValue("value", value_);
}

} // namespace Pattern