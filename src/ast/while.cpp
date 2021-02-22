#include "ast/while.h"

#include "json.h"

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
While::walk(const Func &fn) const {
    Expression::walk(fn);
    cond_->walk(fn);
    block_->walk(fn);
}

const string &
While::getTypeName() const {
    const static string name = "While";
    return name;
}

} // namespace AST
