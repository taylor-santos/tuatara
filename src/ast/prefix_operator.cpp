#include "ast/prefix_operator.h"

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
PrefixOperator::getTypeName() const {
    const static string name = "Prefix Operator";
    return name;
}

void
PrefixOperator::walk(const Node::Func &fn) const {
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
