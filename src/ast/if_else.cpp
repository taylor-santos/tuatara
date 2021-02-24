#include "ast/if_else.h"

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

IfElse::IfElse(
    const yy::location &loc,
    Expression::Ptr     cond,
    Block::Ptr          block,
    Block::Ptr          elseBlock)
    : If(loc, move(cond), move(block))
    , elseBlock_{move(elseBlock)} {}

void
IfElse::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "if else");
    obj.printKeyValue("cond", getCond());
    obj.printKeyValue("if true", getBlock());
    obj.printKeyValue("if false", elseBlock_);
}

void
IfElse::walk(const function<void(const Node &)> &fn) const {
    If::walk(fn);
    elseBlock_->walk(fn);
}

const string &
IfElse::getNodeName() const {
    const static string name = "If Else";
    return name;
}

TypeChecker::Type &
IfElse::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
