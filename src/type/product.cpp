#include "type/product.h"

#include <algorithm>

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using namespace std;

namespace TypeChecker {

Product::Product(yy::location loc, vector<Type::Ptr> types)
    : Type(loc)
    , types_{move(types)} {}

void
Product::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "product");
    obj.printKeyValue("types", types_);
}

void
Product::walk(const std::function<void(const Node &)> &fn) const {
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
        [](const auto &a, const auto &b) { return (*b) <= (*a); });
    return firstDiff.first == types_.end();
}

} // namespace TypeChecker
