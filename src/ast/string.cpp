#include "ast/string.h"

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

String::String(const yy::location &loc, string value)
    : Literal(loc, "string")
    , value_{move(value)} {}

String::~String() = default;

void
String::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "string");
    obj.printKeyValue("value", value_);
}

const string &
String::getNodeName() const {
    const static string name = "String";
    return name;
}

TypeChecker::Type &
String::getTypeImpl(TypeChecker::Context &ctx) {
    auto &type = getMyType();
    type.verify(ctx);
    return type;
}

} // namespace AST
