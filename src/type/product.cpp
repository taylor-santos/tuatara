#include "type/product.h"

#include <algorithm>
#include <utility>

#include "json.h"

using std::function, std::ostream, std::ref, std::reference_wrapper, std::string, std::unique_ptr,
    std::vector;

namespace TypeChecker {

class Context;

Product::Product(yy::location loc, vector<reference_wrapper<Type>> types)
    : Type(loc)
    , types_{move(types)} {}

void
Product::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "product");
    obj.printKeyValue("types", types_);
}

void
Product::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t.get().walk(fn); });
}

const string &
Product::getNodeName() const {
    const static string name = "Product Type";
    return name;
}

void
Product::verifyImpl(Context &ctx) {
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t.get().verify(ctx); });
}
void
Product::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    string sep;
    for (const auto &t : types_) {
        out << sep;
        t.get().pretty(out, false);
        sep = ",";
    }
    if (mod) {
        out << ")";
    }
}

bool
Product::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Product::operator>=(const Product &other) const {
    // Find the first tuple element that isn't a subtype, or the end if they all match.
    auto firstDiff = mismatch(
        types_.begin(),
        types_.end(),
        other.types_.begin(),
        other.types_.end(),
        [](const auto &a, const auto &b) { return (b.get()) <= (a.get()); });
    return firstDiff.first == types_.end();
}

static vector<reference_wrapper<Type>>
makeTypeWrappers(vector<unique_ptr<Type>> &types) {
    vector<reference_wrapper<Type>> out;
    out.reserve(types.size());
    transform(types.begin(), types.end(), back_inserter(out), [](auto &type) {
        return ref(*type);
    });
    return out;
}

ManagedProduct::ManagedProduct(yy::location loc, vector<unique_ptr<Type>> types)
    : Product(loc, makeTypeWrappers(types))
    , types_{move(types)} {}

ManagedProduct::~ManagedProduct() = default;

Product::~Product() = default;

} // namespace TypeChecker
