#include "pattern/type_constraint.h"

#include <cassert>

#include "type/type.h"

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

TypeConstraint::TypeConstraint(const yy::location &loc, shared_ptr<TypeChecker::Type> type)
    : Constraint(loc)
    , type_{move(type)} {
    assert(type_);
}

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

shared_ptr<TypeChecker::Type>
TypeConstraint::getTypeImpl(TypeChecker::Context &ctx) {
    type_ = type_->verify(ctx);
    return type_;
}

} // namespace Pattern
