#include "ast/int.h"
#include "json.h"

using namespace std;

namespace AST {

Int::Int(const yy::location &loc, int64_t value)
    : Expression(loc)
    , value{value} {}

void
Int::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "int");
    obj.KeyValue("value", value);
}

} // namespace AST
