#include "type/sum.h"
#include "json.h"

using namespace std;

namespace TypeChecker {

Sum::Sum(yy::location loc, Type::Vec types)
    : Type(loc)
    , types{move(types)} {}

void
Sum::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "sum");
    obj.Key("types");
    JSON::Array arr(os);
    for (const auto &type : types) {
        arr.Next();
        os << *type;
    }
}

} // namespace TypeChecker
