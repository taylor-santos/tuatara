#include "ast/declaration.h"

#include <utility>

using namespace TuataraAST;
using namespace std;

Declaration::Declaration(const yy::location &loc, string variable)
    : Statement(loc)
    , variable{move(variable)} {}
