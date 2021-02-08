#include "ast/return.h"
#include "json.h"

using namespace std;

namespace AST {

Return::Return(const yy::location &loc)
    : Statement(loc) {}

Return::Return(const yy::location &loc, Expression::Ptr value)
    : Statement(loc)
    , value{move(value)} {}

void
Return::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "return");
    if (value) {
        obj.KeyValue("returns", **value);
    }
}

} // namespace AST