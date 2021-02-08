#include "type/object.h"
#include "json.h"

using namespace TypeChecker;
using namespace std;

Object::Object(yy::location loc, string className)
    : Type(loc)
    , className{move(className)} {}

void
Object::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "object");
    obj.KeyValue("class", className);
}
