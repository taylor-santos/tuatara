#include "type/type.h"

using namespace std;
using namespace TypeChecker;

Type::Type(yy::location loc)
    : loc_{loc} {}

const yy::location &
Type::loc() const {
    return loc_;
}

ostream &
operator<<(ostream &os, const Type &type) {
    type.json(os);
    return os;
}