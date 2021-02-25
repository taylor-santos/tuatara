#include "pattern/type_constraint.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using namespace std;

namespace Pattern {

TypeConstraint::TypeConstraint(const yy::location &loc, TypeChecker::Type::Ptr type)
    : Constraint(loc)
    , type_{move(type)} {}

void
TypeConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "type constraint");
    obj.printKeyValue("type", type_);
}

void
TypeConstraint::walk(const std::function<void(const Node &)> &fn) const {
    Constraint::walk(fn);
    type_->walk(fn);
}

const std::string &
TypeConstraint::getNodeName() const {
    const static string name = "Type Constraint Pattern";
    return name;
}

TypeChecker::Type &
TypeConstraint::getTypeImpl(TypeChecker::Context &ctx) {
    type_->verify(ctx);
    return *type_;
}

} // namespace Pattern
