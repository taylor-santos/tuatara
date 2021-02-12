#include "type/object.h"

#include "json.h"

using namespace TypeChecker;
using namespace std;

Object::Object(yy::location loc, string className)
    : Type(loc)
    , className_{move(className)} {}

void
Object::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "object");
    obj.printKeyValue("class", className_);
}
const string &
Object::getTypeName() const {
    const static string name = "Object Type";
    return name;
}
