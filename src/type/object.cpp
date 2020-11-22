#include "type/object.h"
#include "json.h"

#include <utility>

using namespace TypeChecker;
using namespace std;

Object::Object(yy::location loc, string class_name)
    : Type(loc)
    , class_name{move(class_name)} {}

void
Object::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "object");
    obj.KeyValue("class", class_name);
}
