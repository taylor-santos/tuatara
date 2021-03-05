#include "ast/if.h"

#include <cassert>

#include "type/sum.h"
#include "type/type_context.h"
#include "type/type_exception.h"
#include "type/unit.h"

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

If::If(const yy::location &loc, unique_ptr<Expression> cond, unique_ptr<Block> block)
    : Expression(loc)
    , cond_{move(cond)}
    , block_{move(block)} {
    assert(cond_);
    assert(block_);
}

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

shared_ptr<TypeChecker::Type>
If::getTrueType(TypeChecker::Context &ctx) {
    return block_->getType(ctx);
}

shared_ptr<TypeChecker::Type>
If::getTypeImpl(TypeChecker::Context &ctx) {
    auto condType = cond_->getType(ctx);
    // TODO: check if condType is convertible to bool
    TypeChecker::Context                  newCtx   = ctx;
    auto                                  trueType = getTrueType(newCtx);
    vector<shared_ptr<TypeChecker::Type>> retType{
        trueType,
        make_shared<TypeChecker::Unit>(getLoc())};
    return make_shared<TypeChecker::Sum>(getLoc(), move(retType));
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
