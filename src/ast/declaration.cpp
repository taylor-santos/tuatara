#include "ast/declaration.h"

using namespace std;

namespace AST {

Declaration::Declaration(const yy::location &loc, string variable)
    : Statement(loc)
    , variable{move(variable)} {}

const string &
Declaration::getVariable() const {
    return variable;
}

} // namespace AST