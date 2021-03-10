#include "type/sum.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include "ast/expression.h"

#include "type/array.h"
#include "type/func.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/type_exception.h"
#include "type/unit.h"

#include "json.h"

using std::all_of;
using std::function;
using std::make_shared;
using std::ostream;
using std::pair;
using std::ref;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

class Context;

Sum::Sum(yy::location loc, vector<shared_ptr<Type>> types)
    : Type(loc)
    , types_{move(types)}
    , indexOverride_{make_shared<int>(-1)} {
    assert(all_of(types_.begin(), types_.end(), [](const auto &t) -> bool { return !!t; }));
}

Sum::Sum(yy::location loc, vector<shared_ptr<Type>> types, const shared_ptr<int> &indexOverride)
    : Type(loc)
    , types_{move(types)}
    , indexOverride_{indexOverride} {
    assert(all_of(types_.begin(), types_.end(), [](const auto &t) -> bool { return !!t; }));
}

Sum::~Sum() = default;

shared_ptr<Type>
Sum::clone(const yy::location &loc) const {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    transform(types_.begin(), types_.end(), back_inserter(types), [](const auto t) {
        return t->clone(t->getLoc());
    });
    // Don't clone the indexOverride, make two separate type instances with different assumed types.
    return make_shared<Sum>(loc, move(types));
}

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
    for_each(types_.begin(), types_.end(), [&](auto &t) { t = t->verify(ctx); });
}

void
Sum::pretty(ostream &out, bool mod) const {
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->pretty(out, mod);
    }
    if (mod) {
        out << "(";
    }
    string sep;
    for (auto &t : types_) {
        out << sep;
        t->pretty(out, true);
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
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->isSubtype(other, ctx);
    }
    return other.isSuperImpl(*this, ctx);
}

bool
Sum::isSuper(const Type &other, const Context &ctx) const {
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->isSupertype(other, ctx);
    }
    // A <= (X|Y) iif A <= X || A <= Y
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
Sum::callAsFunc(const Type &arg, const yy::location &loc, Context &ctx) {
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->callAsFunc(arg, loc, ctx);
    }
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (size_t i = 0; i < types_.size(); i++) {
        auto &type = types_[i];
        try {
            IndexOverrider index(*this, i);
            types.push_back(type->callAsFunc(arg, loc, ctx));
        } catch (TypeChecker::TypeException &e) {
            vector<pair<string, yy::location>> msgs;
            {
                stringstream ss;
                ss << "sum type \"";
                Type::pretty(ss);
                ss << "\" cannot be invoked with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            {
                stringstream ss;
                ss << "assuming sum type has type \"";
                type->pretty(ss);
                ss << "\"...";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
            throw TypeChecker::TypeException(msgs);
        }
    }
    auto ret = make_shared<Sum>(getLoc(), types, indexOverride_);
    return ret->verify(ctx);
}

shared_ptr<Type>
Sum::indexAsArray(const Type &arg, const yy::location &loc, Context &ctx) {
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->indexAsArray(arg, loc, ctx);
    }
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (size_t i = 0; i < types_.size(); i++) {
        auto &type = types_[i];
        try {
            IndexOverrider index(*this, i);
            types.push_back(type->indexAsArray(arg, loc, ctx));
        } catch (TypeChecker::TypeException &e) {
            vector<pair<string, yy::location>> msgs;
            {
                stringstream ss;
                ss << "sum type \"";
                Type::pretty(ss);
                ss << "\" cannot be indexed with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            {
                stringstream ss;
                ss << "assuming sum type has type \"";
                type->pretty(ss);
                ss << "\"...";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
            throw TypeChecker::TypeException(msgs);
        }
    }
    auto ret = make_shared<Sum>(getLoc(), types, indexOverride_);
    return ret->verify(ctx);
}

shared_ptr<Type>
Sum::accessField(const string &field, const yy::location &loc, Context &ctx) {
    if (*indexOverride_ != -1) {
        return types_[*indexOverride_]->accessField(field, loc, ctx);
    }
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (size_t i = 0; i < types_.size(); i++) {
        auto &type = types_[i];
        try {
            IndexOverrider index(*this, i);
            types.push_back(type->accessField(field, loc, ctx));
        } catch (TypeChecker::TypeException &e) {
            vector<pair<string, yy::location>> msgs;
            {
                stringstream ss;
                ss << "sum type \"";
                Type::pretty(ss);
                ss << "\" cannot be accessed by field \"" << field << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            {
                stringstream ss;
                ss << "assuming sum type has type \"";
                type->pretty(ss);
                ss << "\"...";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            }
            msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
            throw TypeChecker::TypeException(msgs);
        }
    }
    auto ret = make_shared<Sum>(getLoc(), types, indexOverride_);
    return ret->verify(ctx);
}

void
Sum::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

void
Sum::updateForImpl(Sum &other) const {
    other.indexOverride_ = indexOverride_;
}

Sum::IndexOverrider::IndexOverrider(Sum &sum, int index)
    : sum_{sum} {
    assert(*sum_.indexOverride_ == -1);
    *sum_.indexOverride_ = index;
}

Sum::IndexOverrider::~IndexOverrider() {
    *sum_.indexOverride_ = -1;
}

} // namespace TypeChecker
