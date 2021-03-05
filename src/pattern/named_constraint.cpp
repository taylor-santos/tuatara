#include "pattern/named_constraint.h"

#include <cassert>

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace Pattern {

NamedConstraint::NamedConstraint(
    const yy::location &   loc,
    string                 name,
    unique_ptr<Constraint> constraint)
    : Pattern(loc)
    , name_{move(name)}
    , constraint_{move(constraint)} {
    assert(constraint_);
}

NamedConstraint::~NamedConstraint() = default;

void
NamedConstraint::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "named constraint");
    obj.printKeyValue("name", name_);
    obj.printKeyValue("constraint", constraint_);
}

void
NamedConstraint::walk(const function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    constraint_->walk(fn);
}

const string &
NamedConstraint::getNodeName() const {
    const static string name = "Named Constraint Pattern";
    return name;
}

shared_ptr<TypeChecker::Type>
NamedConstraint::getTypeImpl(TypeChecker::Context &ctx) {
    return constraint_->getType(ctx);
}

const string *
NamedConstraint::getName() {
    return &name_;
}

} // namespace Pattern
