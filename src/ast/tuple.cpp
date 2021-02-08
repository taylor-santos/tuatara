#include "ast/tuple.h"
#include "json.h"

using namespace std;

namespace AST {

Tuple::Tuple(const yy::location &loc, Expression::Vec exprs)
    : LValue(loc)
    , exprs{move(exprs)} {}

void
Tuple::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "tuple");
    obj.Key("expressions");
    { JSON::Array(os) << exprs; }
}

} // namespace AST
