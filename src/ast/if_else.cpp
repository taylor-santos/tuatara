#include "ast/if_else.h"

#include "json.h"

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
IfElse::walk(const Func &fn) const {
    If::walk(fn);
    elseBlock_->walk(fn);
}

const string &
IfElse::getTypeName() const {
    const static string name = "If Else";
    return name;
}

} // namespace AST
