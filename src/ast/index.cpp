#include "ast/index.h"
#include "json.h"

using namespace std;

namespace AST {

Index::Index(const yy::location &loc, Expression::Ptr expr, Expression::Ptr index)
    : LValue(loc)
    , expr{move(expr)}
    , index{move(index)} {}

void
Index::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "array index");
    obj.KeyValue("expr", *expr);
    obj.Key("index");
    os << *index;
}

} // namespace AST
