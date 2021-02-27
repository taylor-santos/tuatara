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

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

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

shared_ptr<TypeChecker::Type>
Block::getTypeImpl(TypeChecker::Context &ctx) {
    shared_ptr<TypeChecker::Type> type;
    for_each(stmts_.begin(), stmts_.end(), [&](auto &s) { type = s->getType(ctx); });
    return type;
}

} // namespace AST
