#include "type/sum.h"

#include <algorithm>

#include "type/type_context.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Sum::Sum(yy::location loc, Type::Vec types)
    : Type(loc)
    , types_{move(types)} {}

void
Sum::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "sum");
    obj.printKeyValue("types", types_);
}

void
Sum::walk(const std::function<void(const Node &)> &fn) const {
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
    for_each(types_.begin(), types_.end(), [&](const auto &t) {
        out << sep;
        t->pretty(out, false);
        sep = ",";
    });
    if (mod) {
        out << ")";
    }
}

bool
Sum::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Sum::operator>=(const Type &other) const {
    return std::any_of(types_.begin(), types_.end(), [&](const auto &type) {
        return other <= (*type);
    });
}

} // namespace TypeChecker
