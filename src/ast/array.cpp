#include "ast/array.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include "type/array.h"
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
using std::pair;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace AST {

Array::Array(const yy::location &loc, vector<unique_ptr<Expression>> exprs)
    : LValue(loc)
    , exprs_{move(exprs)} {
    assert(!exprs_.empty());
    assert(all_of(exprs_.begin(), exprs_.end(), [](const auto &e) -> bool { return !!e; }));
}

Array::~Array() = default;

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "array");
    obj.printKeyValue("expressions", exprs_);
}

void
Array::walk(const function<void(const Node &)> &fn) const {
    LValue::walk(fn);
    for_each(exprs_.begin(), exprs_.end(), [&](const auto &e) { e->walk(fn); });
}

const string &
Array::getNodeName() const {
    const static string name = "Array";
    return name;
}

shared_ptr<TypeChecker::Type>
Array::getTypeImpl(TypeChecker::Context &ctx) {
    auto type = exprs_[0]->getType(ctx);
    for (size_t i = 1; i < exprs_.size(); i++) {
        auto &expr     = exprs_[i];
        auto  exprType = expr->getType(ctx);
        if (!type) {
            type = exprType;
        } else {
            // TODO: Find common ancestor of all elements instead of direct equality
            if (!type->isEqual(*exprType, ctx)) {
                vector<pair<string, yy::location>> msgs;
                msgs.emplace_back("error: array must have uniform type", getLoc());
                {
                    stringstream ss;
                    ss << "note: expression at position " << i << " has type \"";
                    exprType->pretty(ss);
                    ss << "\" which conflicts with the previous elements' type \"";
                    type->pretty(ss);
                    ss << "\"";
                    msgs.emplace_back(ss.str(), expr->getLoc());
                }
                exprType->addTypeLocMessage(msgs, "expression");
                type->addTypeLocMessage(msgs, "first element");
                throw TypeChecker::TypeException(msgs);
            }
        }
    }
    return make_unique<TypeChecker::Array>(getLoc(), type);
}

} // namespace AST
