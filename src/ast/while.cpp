#include "ast/while.h"

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

While::While(const yy::location &loc, Expression::Ptr cond, Block::Ptr block)
    : Expression(loc)
    , cond_{move(cond)}
    , block_{move(block)} {}

void
While::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "while");
    obj.printKeyValue("cond", cond_);
    obj.printKeyValue("block", block_);
}

void
While::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    cond_->walk(fn);
    block_->walk(fn);
}

const string &
While::getNodeName() const {
    const static string name = "While";
    return name;
}

TypeChecker::Type &
While::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
