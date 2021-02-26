#include "ast/if.h"

#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr;

namespace AST {

If::If(const yy::location &loc, unique_ptr<Expression> cond, unique_ptr<Block> block)
    : Expression(loc)
    , cond_{move(cond)}
    , block_{move(block)} {}

If::~If() = default;

void
If::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "if");
    obj.printKeyValue("cond", cond_);
    obj.printKeyValue("if true", block_);
}

void
If::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    cond_->walk(fn);
    block_->walk(fn);
}

const string &
If::getNodeName() const {
    const static string name = "If";
    return name;
}

TypeChecker::Type &
If::getTrueType(TypeChecker::Context &ctx) {
    return block_->getType(ctx);
}

TypeChecker::Type &
If::getTypeImpl(TypeChecker::Context &ctx) {
    auto &trueType = getTrueType(ctx);
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

const Expression &
If::getCond() const {
    return *cond_;
}

const Expression &
If::getBlock() const {
    return *block_;
}

} // namespace AST
