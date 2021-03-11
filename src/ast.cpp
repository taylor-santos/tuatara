#include "ast.h"

using std::function;
using std::ostream;

namespace AST {

Node::Node(const yy::location &loc)
    : loc_(loc) {}

Node::~Node() = default;

ostream &
operator<<(ostream &os, const Node &ast) {
    ast.json(os);
    return os;
}

void
Node::walk(const function<void(const Node &)> &fn) const {
    fn(*this);
}

const yy::location &
Node::getLoc() const {
    return loc_;
}

} // namespace AST
