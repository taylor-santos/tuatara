#include "ast/prefix_operator.h"

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

PrefixOperator::PrefixOperator(const yy::location &loc, std::string op, AST::Expression::Ptr rhs)
    : Operator(loc, std::move(op))
    , rhs_{move(rhs)} {}

const AST::Expression &
AST::PrefixOperator::getRhs() const {
    return *rhs_;
}

const std::string &
PrefixOperator::getNodeName() const {
    const static string name = "Prefix Operator";
    return name;
}

TypeChecker::Type &
PrefixOperator::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

void
PrefixOperator::walk(const std::function<void(const Node &)> &fn) const {
    Operator::walk(fn);
    rhs_->walk(fn);
}

void
PrefixOperator::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "prefix operator");
    obj.printKeyValue("operation", getOp());
    obj.printKeyValue("rhs", rhs_);
}

} // namespace AST
