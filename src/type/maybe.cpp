#include "type/maybe.h"

#include "json.h"

using std::function, std::ostream, std::string, std::unique_ptr;

namespace TypeChecker {

class Context;

Maybe::Maybe(yy::location loc, unique_ptr<Type> type)
    : Type(loc)
    , type_{move(type)} {}

Maybe::~Maybe() = default;

void
Maybe::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "maybe");
    obj.printKeyValue("type", type_);
}

void
Maybe::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    type_->walk(fn);
}

const string &
Maybe::getNodeName() const {
    const static string name = "Maybe Type";
    return name;
}

void
Maybe::verifyImpl(Context &ctx) {
    type_->verify(ctx);
}

void
Maybe::pretty(ostream &out, bool) const {
    type_->pretty(out, true);
    out << "?";
}

bool
Maybe::isSubtype(const Type &other, Context &ctx) const {
    return other.isSupertype(*this, ctx);
}

bool
Maybe::isSupertype(const Type &other, Context &ctx) const {
    return other.isSubtype(*type_, ctx);
}

bool
Maybe::isSupertype(const class Maybe &other, Context &ctx) const {
    return other.type_->isSubtype(*type_, ctx);
}

} // namespace TypeChecker
