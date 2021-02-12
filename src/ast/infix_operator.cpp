#include "ast/infix_operator.h"

#include "json.h"

using namespace std;

namespace AST {

InfixOperator::InfixOperator(
    const yy::location & loc,
    std::string          op,
    AST::Expression::Ptr lhs,
    AST::Expression::Ptr rhs)
    : Operator(loc, op)
    , PrefixOperator(loc, op, move(rhs))
    , PostfixOperator(loc, op, move(lhs)) {}

const std::string &
InfixOperator::getTypeName() const {
    const static string name = "Infix Operator";
    return name;
}

void
InfixOperator::walk(const Node::Func &fn) const {
    PrefixOperator::walk(fn);
    PostfixOperator::walk(fn);
}

void
InfixOperator::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "infix operator");
    obj.printKeyValue("operation", getOp());
    obj.printKeyValue("lhs", getLhs());
    obj.printKeyValue("rhs", getRhs());
}

} // namespace AST
