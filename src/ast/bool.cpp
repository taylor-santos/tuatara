#include "ast/bool.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Bool::Bool(const yy::location &loc, bool value)
    : Literal(loc, "bool")
    , value_{value} {}

void
Bool::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "bool");
    obj.printKeyValue("value", value_);
}

const string &
Bool::getNodeName() const {
    const static string name = "Bool";
    return name;
}

TypeChecker::Type &
Bool::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = getMyType();
    type.verify(ctx);
    return type;
}

} // namespace AST
