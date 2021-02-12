#include "ast/if.h"

#include "json.h"

using namespace std;

namespace AST {

If::If(const yy::location &loc, Expression::Ptr cond, Block::Ptr block)
    : Expression(loc)
    , cond_{move(cond)}
    , block_{move(block)} {}

void
If::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "if");
    obj.printKeyValue("cond", cond_);
    obj.printKeyValue("if true", block_);
}

void
If::walk(const Func &fn) const {
    Expression::walk(fn);
    cond_->walk(fn);
    block_->walk(fn);
}

const string &
If::getTypeName() const {
    const static string name = "If";
    return name;
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
