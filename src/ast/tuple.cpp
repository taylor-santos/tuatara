#include "ast/tuple.h"

#include "json.h"

using namespace std;

namespace AST {

Tuple::Tuple(const yy::location &loc, Expression::Vec exprs)
    : LValue(loc)
    , exprs_{move(exprs)} {}

void
Tuple::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "tuple");
    obj.printKeyValue("expressions", exprs_);
}

void
Tuple::walk(const Func &fn) const {
    LValue::walk(fn);
    for (const auto &expr : exprs_) {
        expr->walk(fn);
    }
}

const string &
Tuple::getTypeName() const {
    const static string name = "Tuple";
    return name;
}

} // namespace AST
