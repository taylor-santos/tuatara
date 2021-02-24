#include "ast/postfix_operator.h"

#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

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
PostfixOperator::getNodeName() const {
    const static string name = "Postfix Operator";
    return name;
}

TypeChecker::Type &
PostfixOperator::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

void
PostfixOperator::walk(const std::function<void(const Node &)> &fn) const {
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
