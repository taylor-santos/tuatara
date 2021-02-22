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
NamedConstraint::walk(const AST::Node::Func &fn) const {
    Pattern::walk(fn);
    constraint_->walk(fn);
}

const std::string &
NamedConstraint::getTypeName() const {
    const static string name = "Named Constraint Pattern";
    return name;
}
void
NamedConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named constraint");
    obj.printKeyValue("name", name_);
    obj.printKeyValue("constraint", constraint_);
}

} // namespace Pattern