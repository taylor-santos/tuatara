#include "type/sum.h"

#include <common.h>

#include <algorithm>

#include "json.h"
#include "toRefs.h"

using std::function, std::ostream, std::ref, std::reference_wrapper, std::string, std::unique_ptr,
    std::vector;

namespace TypeChecker {

class Context;

Sum::Sum(yy::location loc, vector<reference_wrapper<Type>> types)
    : Type(loc)
    , types_{move(types)} {}

Sum::Sum(yy::location loc, std::vector<std::unique_ptr<Type>> types)
    : Type(loc)
    , ownedTypes_{move(types)}
    , types_{toRefs(ownedTypes_)} {}

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
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t.get().walk(fn); });
}

const string &
Sum::getNodeName() const {
    const static string name = "Sum Type";
    return name;
}

void
Sum::verifyImpl(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t.get().verify(ctx); });
}

void
Sum::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    string sep;
    for (auto &t : types_) {
        out << sep;
        t.get().pretty(out, false);
        sep = "|";
    }
    if (mod) {
        out << ")";
    }
}

bool
Sum::isSubtype(const Type &other, Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Sum::isSuper(const Type &other, Context &ctx) const {
    return any_of(types_.begin(), types_.end(), [&](const auto &type) {
        return other.isSubtype(type.get(), ctx);
    });
}

bool
Sum::isSuperImpl(const TypeChecker::Array &other, Context &ctx) const {
    return isSuper(other, ctx);
}

bool
Sum::isSuperImpl(const TypeChecker::Func &other, Context &ctx) const {
    return isSuper(other, ctx);
}
bool
Sum::isSuperImpl(const TypeChecker::Maybe &other, Context &ctx) const {
    return isSuper(other, ctx);
}
bool
Sum::isSuperImpl(const TypeChecker::Object &other, Context &ctx) const {
    return isSuper(other, ctx);
}
bool
Sum::isSuperImpl(const TypeChecker::Product &other, Context &ctx) const {
    return isSuper(other, ctx);
}
bool
Sum::isSuperImpl(const Sum &other, Context &ctx) const {
    return isSuper(other, ctx);
}
bool
Sum::isSuperImpl(const TypeChecker::Unit &other, Context &ctx) const {
    return isSuper(other, ctx);
}

} // namespace TypeChecker
