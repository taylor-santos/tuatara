#include "type/type.h"

using namespace std;
using namespace TypeChecker;

Type::Type(yy::location loc)
    : loc{loc} {}

ostream &
operator<<(ostream &os, const Type &type) {
    type.json(os);
    return os;
}