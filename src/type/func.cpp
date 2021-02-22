#include "type/func.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Func::Func(yy::location loc, Type::Ptr argType, Type::Ptr retType)
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
    argType_->walk(fn);
    retType_->walk(fn);
}

const string &
Func::getTypeName() const {
    const static string name = "Func Type";
    return name;
}

} // namespace TypeChecker
