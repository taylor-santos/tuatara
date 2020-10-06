#include "ast/ast.h"

using namespace TuataraAST;

AST::AST(const yy::location &loc)
    : loc(loc) {}

std::ostream &
operator<<(std::ostream &os, const TuataraAST::AST &ast) {
    ast.json(os);
    return os;
}