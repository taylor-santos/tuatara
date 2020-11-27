#include "type/array.h"
#include "json.h"

#include <utility>

using namespace TypeChecker;
using namespace std;

Array::Array(yy::location loc, Type::Ptr type)
    : Type(loc)
    , type{move(type)} {}

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "array");
    obj.KeyValue("type", *type);
}
