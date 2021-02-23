#include "ast/tuple.h"

#include <algorithm>

#include "type/type_exception.h"

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
Tuple::walk(const function<void(const Node &)> &fn) const {
    LValue::walk(fn);
    for_each(exprs_.begin(), exprs_.end(), [&](const auto &e) { e->walk(fn); });
}

const string &
Tuple::getNodeName() const {
    const static string name = "Tuple";
    return name;
}

TypeChecker::Type &
Tuple::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
