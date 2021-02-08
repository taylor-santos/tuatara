#include "ast/operator.h"
#include "json.h"

using namespace std;

namespace AST {

Operator::Operator(const yy::location &loc, string op, Expression::Ptr lhs, Expression::Ptr rhs)
    : LValue(loc)
    , op{move(op)}
    , lhs{move(lhs)}
    , rhs{move(rhs)} {}

void
Operator::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "operator");
    obj.KeyValue("operation", op);
    obj.KeyValue("lhs", *lhs);
    obj.KeyValue("rhs", *rhs);
}

} // namespace AST