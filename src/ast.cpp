#include "ast.h"

using namespace std;

namespace AST {

Node::Node(const yy::location &loc)
    : loc_(loc) {}

ostream &
operator<<(ostream &os, const Node &ast) {
    ast.json(os);
    return os;
}

void
Node::walk(const std::function<void(const Node &)> &fn) const {
    fn(*this);
}

const yy::location &
Node::getLoc() const {
    return loc_;
}

void
Node::setLoc(const yy::location &loc) {
    loc_ = loc;
}

} // namespace AST
