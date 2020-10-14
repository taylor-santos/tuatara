#include "ast/ast.h"

using namespace AST;
using namespace std;

Node::Node(const yy::location &loc)
    : loc(loc) {}

ostream &
operator<<(ostream &os, const Node &ast) {
    ast.json(os);
    return os;
}