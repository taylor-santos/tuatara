#include "ast/ast.h"

using namespace TuataraAST;
using namespace std;

AST::AST(const yy::location &loc)
    : loc(loc) {}

ostream &
operator<<(ostream &os, const TuataraAST::AST &ast) {
    ast.json(os);
    return os;
}