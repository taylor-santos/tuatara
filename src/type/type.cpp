#include "type/type.h"

using namespace std;

namespace TypeChecker {

Type::Type(yy::location loc)
    : loc{loc} {}

const yy::location &
Type::getLoc() const {
    return loc;
}

ostream &
operator<<(ostream &os, const Type &type) {
    type.json(os);
    return os;
}

} // namespace TypeChecker