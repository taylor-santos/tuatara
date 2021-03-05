#include "ast/if_else.h"

#include <cassert>

#include "type/sum.h"
#include "type/type_context.h"
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

IfElse::IfElse(
    const yy::location &   loc,
    unique_ptr<Expression> cond,
    unique_ptr<Block>      block,
    unique_ptr<Block>      elseBlock)
    : If(loc, move(cond), move(block))
    , elseBlock_{move(elseBlock)} {
    assert(elseBlock_);
}

IfElse::~IfElse() = default;

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

shared_ptr<TypeChecker::Type>
IfElse::getTypeImpl(TypeChecker::Context &ctx) {
    auto                                  trueType  = getTrueType(ctx);
    TypeChecker::Context                  newCtx    = ctx;
    auto                                  falseType = elseBlock_->getType(newCtx);
    vector<shared_ptr<TypeChecker::Type>> retType{trueType, falseType};
    return make_shared<TypeChecker::Sum>(getLoc(), retType);
}

} // namespace AST
