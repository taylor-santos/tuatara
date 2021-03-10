#include "ast/index.h"

#include <cassert>

#include "type/type.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

Index::Index(const yy::location &loc, unique_ptr<Expression> expr, unique_ptr<Expression> index)
    : LValue(loc)
    , expr_{move(expr)}
    , index_{move(index)} {
    assert(expr_);
    assert(index_);
}

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

shared_ptr<TypeChecker::Type>
Index::getTypeImpl(TypeChecker::Context &ctx) {
    auto type      = expr_->getType(ctx);
    auto indexType = index_->getType(ctx);
    return type->indexAsArray(*indexType, getLoc(), ctx);
}

} // namespace AST
