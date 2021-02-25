#include "ast/float.h"

#include "type/object.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::ostream, std::string;

namespace AST {

Float::Float(const yy::location &loc, double value)
    : Literal(loc, "float")
    , value_{value} {}

Float::~Float() = default;

void
Float::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "float");
    obj.printKeyValue("value", value_);
}

const string &
Float::getNodeName() const {
    const static string name = "Float";
    return name;
}

TypeChecker::Type &
Float::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = getMyType();
    type.verify(ctx);
    return type;
}

} // namespace AST
