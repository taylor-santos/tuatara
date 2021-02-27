#include "pattern/value_constraint.h"

#include "ast/expression.h"

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

ValueConstraint::ValueConstraint(const yy::location &loc, unique_ptr<AST::Expression> value)
    : Constraint(loc)
    , value_{move(value)} {}

ValueConstraint::~ValueConstraint() = default;

void
ValueConstraint::walk(const function<void(const Node &)> &fn) const {
    Constraint::walk(fn);
    value_->walk(fn);
}

void
ValueConstraint::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "value constraint");
    obj.printKeyValue("value", value_);
}

const string &
ValueConstraint::getNodeName() const {
    const static string name = "Value Constraint Pattern";
    return name;
}

shared_ptr<TypeChecker::Type>
ValueConstraint::getTypeImpl(TypeChecker::Context &ctx) {
    return value_->getType(ctx);
}

} // namespace Pattern
