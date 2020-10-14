#include "ast/declaration.h"

#include <utility>

using namespace AST;
using namespace std;

Declaration::Declaration(const yy::location &loc, string variable)
    : Statement(loc)
    , variable{move(variable)} {}
