#include "type/func.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Func::Func(yy::location loc, optional<Type::Ptr> argType, optional<Type::Ptr> retType)
    : Type(loc)
    , argType_{move(argType)}
    , retType_{move(retType)} {}

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "func");
    obj.printKeyValue("arg", argType_);
    obj.printKeyValue("return type", retType_);
}

void
Func::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
    if (argType_) {
        (*argType_)->walk(fn);
    }
    if (retType_) {
        (*retType_)->walk(fn);
    }
}

const string &
Func::getTypeName() const {
    const static string name = "Func Type";
    return name;
}

} // namespace TypeChecker
