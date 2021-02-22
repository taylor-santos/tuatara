#include "ast/block.h"

#include "json.h"

using namespace std;

namespace AST {

Block::Block(const yy::location &loc, Expression::Vec stmts)
    : Expression(loc)
    , stmts_{move(stmts)} {}

void
Block::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "block");
    obj.printKeyValue("statements", stmts_);
}

void
Block::walk(const Func &fn) const {
    Expression::walk(fn);
    for (const auto &stmt : stmts_) {
        stmt->walk(fn);
    }
}

const string &
Block::getTypeName() const {
    const static string name = "Block";
    return name;
}

} // namespace AST
