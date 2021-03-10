#include "type/product.h"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <utility>

#include "ast/expression.h"

#include "type/type_exception.h"

#include "json.h"

using std::back_inserter;
using std::for_each;
using std::function;
using std::make_shared;
using std::ostream;
using std::pair;
using std::ref;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::transform;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

class Context;

Product::Product(yy::location loc, vector<shared_ptr<Type>> types)
    : Type(loc)
    , types_{move(types)} {
    assert(all_of(types_.begin(), types_.end(), [](const auto &t) -> bool { return !!t; }));
}

Product::~Product() = default;

shared_ptr<Type>
Product::clone(const yy::location &loc) const {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    transform(types_.begin(), types_.end(), back_inserter(types), [](const auto t) {
        return t->clone(t->getLoc());
    });
    return make_shared<Product>(loc, move(types));
}

void
Product::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "product");
    obj.printKeyValue("types", types_);
}

void
Product::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t->walk(fn); });
}

const string &
Product::getNodeName() const {
    const static string name = "Product Type";
    return name;
}

void
Product::verifyImpl(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](auto &t) { t = t->verify(ctx); });
}
void
Product::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    string sep;
    for (const auto &t : types_) {
        out << sep;
        t->pretty(out, true);
        sep = ",";
    }
    if (mod) {
        out << ")";
    }
}

shared_ptr<Type>
Product::simplify(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](auto &type) { type = type->simplify(ctx); });
    return shared_from_this();
}

bool
Product::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Product::isSuperImpl(const class Product &other, const Context &ctx) const {
    // Find the first tuple element that isn't a subtype, or the end if they all match.
    auto firstDiff = mismatch(
        types_.begin(),
        types_.end(),
        other.types_.begin(),
        other.types_.end(),
        [&](const auto &a, const auto &b) { return b->isSubtype(*a, ctx); });
    return firstDiff.first == types_.end();
}

shared_ptr<Type>
Product::callAsFunc(const Type &arg, const yy::location &loc, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    auto argProduct = dynamic_cast<const Product *>(&arg);
    if (argProduct) {
        if (argProduct->types_.size() != types_.size()) {
            vector<pair<string, yy::location>> msgs;
            {
                stringstream ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be invoked with tuple \"";
                arg.pretty(ss);
                ss << "\" because it has " << argProduct->types_.size() << " elements, but "
                   << types_.size() << " were expected";
                msgs.emplace_back(ss.str(), arg.getLoc());
            }
            addTypeLocMessage(msgs, "tuple");
            throw TypeChecker::TypeException(msgs);
        }
        for (size_t i = 0; i < types_.size(); i++) {
            auto &type    = types_[i];
            auto &argType = argProduct->types_[i];
            try {
                types.push_back(type->callAsFunc(*argType, loc, ctx));
            } catch (TypeChecker::TypeException &e) {
                vector<pair<string, yy::location>> msgs;
                stringstream                       ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be invoked with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
                msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
                throw TypeChecker::TypeException(msgs);
            }
        }
    } else {
        for (const auto &type : types_) {
            try {
                types.push_back(type->callAsFunc(arg, loc, ctx));
            } catch (TypeChecker::TypeException &e) {
                vector<pair<string, yy::location>> msgs;
                stringstream                       ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be invoked with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
                msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
                throw TypeChecker::TypeException(msgs);
            }
        }
    }
    auto ret = make_shared<Product>(loc, types);
    return ret->verify(ctx);
}

shared_ptr<Type>
Product::indexAsArray(const Type &arg, const yy::location &loc, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    auto argProduct = dynamic_cast<const Product *>(&arg);
    if (argProduct) {
        if (argProduct->types_.size() != types_.size()) {
            vector<pair<string, yy::location>> msgs;
            {
                stringstream ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be indexed with tuple \"";
                arg.pretty(ss);
                ss << "\" because it has " << argProduct->types_.size() << " elements, but "
                   << types_.size() << " were expected";
                msgs.emplace_back(ss.str(), arg.getLoc());
            }
            addTypeLocMessage(msgs, "tuple");
            throw TypeChecker::TypeException(msgs);
        }
        for (size_t i = 0; i < types_.size(); i++) {
            auto &type    = types_[i];
            auto &argType = argProduct->types_[i];
            try {
                types.push_back(type->indexAsArray(*argType, loc, ctx));
            } catch (TypeChecker::TypeException &e) {
                vector<pair<string, yy::location>> msgs;
                stringstream                       ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be indexed with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
                msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
                throw TypeChecker::TypeException(msgs);
            }
        }
    } else {
        for (const auto &type : types_) {
            try {
                types.push_back(type->indexAsArray(arg, loc, ctx));
            } catch (TypeChecker::TypeException &e) {
                vector<pair<string, yy::location>> msgs;
                stringstream                       ss;
                ss << "product type \"";
                Type::pretty(ss);
                ss << "\" cannot be indexed with argument type \"";
                arg.pretty(ss);
                ss << "\" because:";
                msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
                msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
                throw TypeChecker::TypeException(msgs);
            }
        }
    }
    auto ret = make_shared<Product>(loc, types);
    return ret->verify(ctx);
}

shared_ptr<Type>
Product::accessField(const string &field, const yy::location &loc, Context &ctx) {
    vector<shared_ptr<Type>> types;
    types.reserve(types_.size());
    for (const auto &type : types_) {
        try {
            types.push_back(type->accessField(field, loc, ctx));
        } catch (TypeChecker::TypeException &e) {
            vector<pair<string, yy::location>> msgs;
            stringstream                       ss;
            ss << "product type \"";
            Type::pretty(ss);
            ss << "\" cannot be accessed by field \"" << field << "\" because:";
            msgs.emplace_back(ss.str(), yy::location{nullptr, 0, 0});
            msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
            throw TypeChecker::TypeException(msgs);
        }
    }
    auto ret = make_shared<Product>(loc, types);
    return ret->verify(ctx);
}

void
Product::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

} // namespace TypeChecker
