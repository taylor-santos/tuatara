#include "ast/tuple.h"

#include <algorithm>
#include <cassert>

#include "type/product.h"
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
using std::make_unique;
using std::ostream;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace AST {

Tuple::Tuple(const yy::location &loc, vector<unique_ptr<Expression>> exprs)
    : LValue(loc)
    , exprs_{move(exprs)} {
    assert(all_of(exprs_.begin(), exprs_.end(), [](const auto &e) -> bool { return !!e; }));
}

Tuple::~Tuple() = default;

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

shared_ptr<TypeChecker::Type>
Tuple::getTypeImpl(TypeChecker::Context &ctx) {
    vector<shared_ptr<TypeChecker::Type>> types;
    types.reserve(exprs_.size());
    for (auto &expr : exprs_) {
        types.emplace_back(expr->getType(ctx));
    }
    return make_shared<TypeChecker::Product>(getLoc(), move(types));
}

} // namespace AST
