#include "ast/postfix_operator.h"

#include "json.h"

using namespace std;

namespace AST {

PostfixOperator::PostfixOperator(const yy::location &loc, std::string op, AST::Expression::Ptr lhs)
    : Operator(loc, std::move(op))
    , lhs_{move(lhs)} {}

const AST::Expression &
AST::PostfixOperator::getLhs() const {
    return *lhs_;
}

const std::string &
PostfixOperator::getTypeName() const {
    const static string name = "Postfix Operator";
    return name;
}

void
PostfixOperator::walk(const Node::Func &fn) const {
    Operator::walk(fn);
    lhs_->walk(fn);
}

void
PostfixOperator::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "postfix operator");
    obj.printKeyValue("operation", getOp());
    obj.printKeyValue("lhs", lhs_);
}

} // namespace AST
