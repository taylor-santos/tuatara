#include "type/array.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::function, std::ostream, std::string, std::unique_ptr;

namespace TypeChecker {

Array::Array(yy::location loc, unique_ptr<Type> type)
    : Type(loc)
    , type_{move(type)} {}

Array::~Array() = default;

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "array");
    obj.printKeyValue("type", type_);
}

void
Array::walk(const function<void(const Node &)> &fn) const {
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
Array::isSubtype(const Type &other, Context &ctx) const {
    return other.isSupertype(*this, ctx);
}

bool
Array::isSupertype(const Type &other, Context &ctx) const {
    return other.isSubtype(*this, ctx);
}

bool
Array::isSupertype(const class Array &other, Context &ctx) const {
    return other.type_->isSubtype(*type_, ctx);
}

} // namespace TypeChecker
