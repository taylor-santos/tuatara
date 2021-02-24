#include "ast/int.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Int::Int(const yy::location &loc, int64_t value)
    : Literal(loc, "int")
    , value_{value} {}

void
Int::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "int");
    obj.printKeyValue("value", value_);
}

const string &
Int::getNodeName() const {
    const static string name = "Int";
    return name;
}

TypeChecker::Type &
Int::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = getMyType();
    type.verify(ctx);
    return type;
}

} // namespace AST
