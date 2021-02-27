#include "ast/int.h"

#include "type/object.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;

namespace AST {

Int::Int(const yy::location &loc, int64_t value)
    : Literal(loc, "int")
    , value_{value} {}

Int::~Int() = default;

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

} // namespace AST
