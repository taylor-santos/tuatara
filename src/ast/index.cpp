#include "ast/index.h"

#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr;

namespace AST {

Index::Index(const yy::location &loc, unique_ptr<Expression> expr, unique_ptr<Expression> index)
    : LValue(loc)
    , expr_{move(expr)}
    , index_{move(index)} {}

Index::~Index() = default;

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
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
