#include "type/sum.h"

#include <algorithm>

#include "json.h"

using std::function, std::ostream, std::string, std::unique_ptr, std::vector;

namespace TypeChecker {

class Context;

Sum::Sum(yy::location loc, vector<unique_ptr<Type>> types)
    : Type(loc)
    , types_{move(types)} {}

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
    for_each(types_.begin(), types_.end(), [&](const auto &t) { t->verify(ctx); });
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
        sep = ",";
    }
    if (mod) {
        out << ")";
    }
}

bool
Sum::isSubtype(const Type &other) const {
    return other.isSupertype(*this);
}

bool
Sum::isSupertype(const Type &other) const {
    return any_of(types_.begin(), types_.end(), [&](const auto &type) {
        return other.isSubtype(*type);
    });
}

} // namespace TypeChecker
