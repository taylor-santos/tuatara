#include "ast/ast.h"

using namespace std;

namespace AST {

Node::Node(const yy::location &loc)
    : loc(loc) {}

ostream &
operator<<(ostream &os, const Node &ast) {
    ast.json(os);
    return os;
}

} // namespace AST