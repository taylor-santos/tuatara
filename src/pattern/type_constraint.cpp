#include "pattern/type_constraint.h"

#include "type/type.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr;

namespace Pattern {

TypeConstraint::TypeConstraint(const yy::location &loc, unique_ptr<TypeChecker::Type> type)
    : Constraint(loc)
    , type_{move(type)} {}

TypeConstraint::~TypeConstraint() = default;

void
TypeConstraint::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "type constraint");
    obj.printKeyValue("type", type_);
}

void
TypeConstraint::walk(const function<void(const Node &)> &fn) const {
    Constraint::walk(fn);
    type_->walk(fn);
}

const string &
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
