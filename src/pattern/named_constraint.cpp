#include "pattern/named_constraint.h"

#include "json.h"

using namespace std;

namespace Pattern {

NamedConstraint::NamedConstraint(
    const yy::location &loc,
    std::string         name,
    Constraint::Ptr     constraint)
    : Pattern(loc)
    , name_{std::move(name)}
    , constraint_{move(constraint)} {}

void
NamedConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named constraint");
    obj.printKeyValue("name", name_);
    obj.printKeyValue("constraint", constraint_);
}

void
NamedConstraint::walk(const std::function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    constraint_->walk(fn);
}

const std::string &
NamedConstraint::getNodeName() const {
    const static string name = "Named Constraint Pattern";
    return name;
}

} // namespace Pattern