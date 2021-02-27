#include "type/product.h"

#include <algorithm>
#include <utility>

#include "json.h"
#include "toRefs.h"

using std::for_each;
using std::function;
using std::make_shared;
using std::ostream;
using std::ref;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

class Context;

Product::Product(yy::location loc, vector<shared_ptr<Type>> types)
    : Type(loc)
    , types_{move(types)} {}

Product::~Product() = default;

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
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t->verify(ctx); });
}
void
Product::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    string sep;
    for (const auto &t : types_) {
        out << sep;
        t->pretty(out, false);
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

} // namespace TypeChecker
