#include "type/sum.h"

#include <algorithm>
#include <cassert>

#include "type/array.h"
#include "type/func.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/unit.h"

#include "json.h"

using std::all_of;
using std::function;
using std::make_shared;
using std::ostream;
using std::ref;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::unique;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

class Context;

Sum::Sum(yy::location loc, vector<shared_ptr<Type>> types)
    : Type(loc)
    , types_{move(types)} {
    assert(all_of(types_.begin(), types_.end(), [](const auto &t) -> bool { return !!t; }));
}

Sum::~Sum() = default;

void
Sum::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "sum");
    obj.printKeyValue("types", types_);
}

void
Sum::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t->walk(fn); });
}

const string &
Sum::getNodeName() const {
    const static string name = "Sum Type";
    return name;
}

void
Sum::verifyImpl(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](auto &t) { t = TypeChecker::Type::verify(t, ctx); });
}

void
Sum::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    string sep;
    for (auto &t : types_) {
        out << sep;
        t->pretty(out, false);
        sep = "|";
    }
    if (mod) {
        out << ")";
    }
}

shared_ptr<Type>
Sum::simplify(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](auto &type) { type = type->simplify(ctx); });
    types_.erase(
        unique(
            types_.begin(),
            types_.end(),
            [&](const auto &a, const auto &b) { return a->isEqual(*b, ctx); }),
        types_.end());
    if (types_.size() == 1) {
        return types_[0];
    }
    return shared_from_this();
}

bool
Sum::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Sum::isSuper(const Type &other, const Context &ctx) const {
    // A <= (A|B) iif A <= A
    return any_of(types_.begin(), types_.end(), [&](const auto &type) {
        return other.isSubtype(*type, ctx);
    });
}

bool
Sum::isSuperImpl(const Sum &other, const Context &ctx) const {
    // (A|B) <= (X|Y|Z) iif A <= (X|Y|Z) && B <= (X|Y|Z)
    return all_of(other.types_.begin(), other.types_.end(), [&](const auto &type) {
        return isSuper(*type, ctx);
    });
}

bool
Sum::isSuperImpl(const Array &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const Func &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const Maybe &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const Object &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const Product &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const Unit &other, const Context &ctx) const {
    return isSuper(other, ctx);
}

shared_ptr<Type>
Sum::callAsFunc(const Type &arg, const AST::Expression &call, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (const auto &type : types_) {
        types.push_back(type->callAsFunc(arg, call, ctx));
    }
    auto ret = make_shared<Sum>(getLoc(), types);
    return ret->simplify(ctx);
}

shared_ptr<Type>
Sum::indexAsArray(AST::Expression &arg, const AST::Expression &index, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (const auto &type : types_) {
        types.push_back(type->indexAsArray(arg, index, ctx));
    }
    auto ret = make_shared<Sum>(getLoc(), types);
    return ret->simplify(ctx);
}

shared_ptr<Type>
Sum::accessField(const string &field, const AST::Expression &access, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (const auto &type : types_) {
        types.push_back(type->accessField(field, access, ctx));
    }
    auto ret = make_shared<Sum>(getLoc(), types);
    return ret->simplify(ctx);
}

} // namespace TypeChecker
