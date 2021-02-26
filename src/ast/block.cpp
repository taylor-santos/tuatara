#include "ast/block.h"

#include <algorithm>

#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr, std::vector;

namespace AST {

Block::Block(const yy::location &loc, vector<unique_ptr<Expression>> stmts)
    : Expression(loc)
    , stmts_{move(stmts)} {}

Block::~Block() = default;

void
Block::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "block");
    obj.printKeyValue("statements", stmts_);
}

void
Block::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    for_each(stmts_.begin(), stmts_.end(), [&](const auto &s) { s->walk(fn); });
}

const string &
Block::getNodeName() const {
    const static string name = "Block";
    return name;
}

TypeChecker::Type &
Block::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
