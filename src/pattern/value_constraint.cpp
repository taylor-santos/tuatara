#include "pattern/value_constraint.h"

#include "ast/expression.h"

#include "json.h"

using namespace std;

namespace Pattern {

ValueConstraint::ValueConstraint(const yy::location &loc, AST::Expression::Ptr value)
    : Constraint(loc)
    , value_{move(value)} {}

void
ValueConstraint::walk(const std::function<void(const Node &)> &fn) const {
    Constraint::walk(fn);
    value_->walk(fn);
}

void
ValueConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "value constraint");
    obj.printKeyValue("value", value_);
}

const std::string &
ValueConstraint::getNodeName() const {
    const static string name = "Value Constraint Pattern";
    return name;
}

} // namespace Pattern
