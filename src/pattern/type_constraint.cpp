#include "pattern/type_constraint.h"

#include "json.h"

using namespace std;

namespace Pattern {

TypeConstraint::TypeConstraint(const yy::location &loc, TypeChecker::Type::Ptr type)
    : Constraint(loc)
    , type_{move(type)} {}

void
TypeConstraint::walk(const AST::Node::Func &fn) const {
    Constraint::walk(fn);
    type_->walk(fn);
}

const std::string &
TypeConstraint::getTypeName() const {
    const static string name = "Type Constraint Pattern";
    return name;
}
void
TypeConstraint::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "type constraint");
    obj.printKeyValue("type", type_);
}

} // namespace Pattern