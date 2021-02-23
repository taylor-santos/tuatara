#include "ast/int.h"

#include "type/object.h"
#include "type/type_exception.h"

#include "json.h"

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
