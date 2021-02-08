#include "type/func.h"
#include "json.h"

using namespace std;

namespace TypeChecker {

Func::Func(yy::location loc, optional<Type::Ptr> argType, optional<Type::Ptr> retType)
    : Type(loc)
    , argType{move(argType)}
    , retType{move(retType)} {}

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "func");
    if (argType) {
        obj.KeyValue("arg", **argType);
    }
    if (retType) {
        obj.KeyValue("returns", **retType);
    }
}

} // namespace TypeChecker
