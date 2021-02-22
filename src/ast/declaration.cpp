#include "ast/declaration.h"

using namespace std;

namespace AST {

Declaration::Declaration(const yy::location &loc, string variable)
    : Expression(loc)
    , variable_{move(variable)} {}

const string &
Declaration::getVariable() const {
    return variable_;
}

} // namespace AST
