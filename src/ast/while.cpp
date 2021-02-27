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

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

While::While(const yy::location &loc, unique_ptr<Expression> cond, unique_ptr<Block> block)
    : Expression(loc)
    , cond_{move(cond)}
    , block_{move(block)} {}

While::~While() = default;

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

shared_ptr<TypeChecker::Type>
While::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
