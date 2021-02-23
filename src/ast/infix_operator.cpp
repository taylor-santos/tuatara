#include "ast/infix_operator.h"

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

InfixOperator::InfixOperator(
    const yy::location & loc,
    const std::string &  op,
    AST::Expression::Ptr lhs,
    AST::Expression::Ptr rhs)
    : Operator(loc, op)
    , PrefixOperator(loc, op, move(rhs))
    , PostfixOperator(loc, op, move(lhs)) {}

const std::string &
InfixOperator::getNodeName() const {
    const static string name = "Infix Operator";
    return name;
}

TypeChecker::Type &
InfixOperator::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

void
InfixOperator::walk(const std::function<void(const Node &)> &fn) const {
    Operator::walk(fn);
    getLhs().walk(fn);
    getRhs().walk(fn);
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
