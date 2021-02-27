#include "type/array.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace TypeChecker {

Array::Array(yy::location loc, shared_ptr<Type> type)
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
Array::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Array::isSuperImpl(const class Array &other, const Context &ctx) const {
    return other.type_->isSubtype(*type_, ctx);
}

shared_ptr<Type>
Array::simplify(Context &ctx) {
    type_ = type_->simplify(ctx);
    return shared_from_this();
}

} // namespace TypeChecker
