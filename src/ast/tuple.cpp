#include "ast/tuple.h"
#include "json.h"

using namespace AST;
using namespace std;

Tuple::Tuple(const yy::location &loc, Expression::Vec exprs)
    : LValue(loc)
    , exprs{move(exprs)} {}

void
Tuple::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "tuple");
    obj.Key("expressions");
    {
        JSON::Array arr(os);
        for (const auto &expr : exprs) {
            arr.Next();
            os << *expr;
        }
    }
}
