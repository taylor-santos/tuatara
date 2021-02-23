#include "ast/index.h"

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

Index::Index(const yy::location &loc, Expression::Ptr expr, Expression::Ptr index)
    : LValue(loc)
    , expr_{move(expr)}
    , index_{move(index)} {}

void
Index::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "array index");
    obj.printKeyValue("expr", expr_);
    obj.printKeyValue("index", index_);
}

void
Index::walk(const function<void(const Node &)> &fn) const {
    LValue::walk(fn);
    expr_->walk(fn);
    index_->walk(fn);
}

const string &
Index::getNodeName() const {
    const static string name = "Index";
    return name;
}

TypeChecker::Type &
Index::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
