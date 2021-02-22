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
Node::walk(const Func &fn) const {
    fn(*this);
}

const yy::location &
Node::getLoc() const {
    return loc_;
}

} // namespace AST
