#include "type/array.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using namespace std;

namespace TypeChecker {

Array::Array(yy::location loc, Type::Ptr type)
    : Type(loc)
    , type_{move(type)} {}

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "array");
    obj.printKeyValue("type", type_);
}

void
Array::walk(const std::function<void(const Node &)> &fn) const {
    Type::walk(fn);
    type_->walk(fn);
}

const string &
Array::getNodeName() const {
    const static string name = "Array Type";
    return name;
}

void
Array::verifyImpl(Context &ctx) {
    type_->verify(ctx);
}

void
Array::pretty(ostream &out, bool) const {
    type_->pretty(out, true);
    out << "[]";
}

bool
Array::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Array::operator>=(const Array &other) const {
    return (*other.type_) <= (*type_);
}

} // namespace TypeChecker
