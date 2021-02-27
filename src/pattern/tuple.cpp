#include "pattern/tuple.h"

#include <algorithm>

#include "type/product.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::make_unique;
using std::ostream;
using std::reference_wrapper;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace Pattern {

Tuple::Tuple(const yy::location &loc, vector<unique_ptr<Pattern>> patterns)
    : Pattern(loc)
    , patterns_{move(patterns)} {}

Tuple::~Tuple() = default;

void
Tuple::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "tuple");
    obj.printKeyValue("patterns", patterns_);
}

void
Tuple::walk(const function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    for_each(patterns_.begin(), patterns_.end(), [&](const auto &p) { p->walk(fn); });
}

const string &
Tuple::getNodeName() const {
    const static string name = "Tuple Pattern";
    return name;
}

shared_ptr<TypeChecker::Type>
Tuple::getTypeImpl(TypeChecker::Context &ctx) {
    vector<shared_ptr<TypeChecker::Type>> types;
    types.reserve(patterns_.size());
    for (const auto &pattern : patterns_) {
        types.emplace_back(pattern->getType(ctx));
    }
    type_ = make_shared<TypeChecker::Product>(getLoc(), move(types));
    return type_;
}

} // namespace Pattern
