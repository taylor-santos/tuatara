#include "type/tuple.h"
#include "json.h"

using namespace TypeChecker;
using namespace std;

Tuple::Tuple(yy::location loc, vector<shared_ptr<Type>> types)
    : Type(loc)
    , types{move(types)} {}

void
Tuple::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "tuple");
    obj.Key("types");
    JSON::Array arr(os);
    for (const auto &type : types) {
        arr.Next();
        os << *type;
    }
}
