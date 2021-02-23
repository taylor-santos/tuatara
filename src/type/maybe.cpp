#include "type/maybe.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Maybe::Maybe(yy::location loc, Type::Ptr type)
    : Type(loc)
    , type_{move(type)} {}

void
Maybe::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "maybe");
    obj.printKeyValue("type", type_);
}

void
Maybe::walk(const std::function<void(const Node &)> &fn) const {
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
Maybe::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Maybe::operator>=(const Type &other) const {
    return other <= (*type_);
}

bool
Maybe::operator>=(const Maybe &other) const {
    return (*other.type_) <= (*type_);
}

} // namespace TypeChecker
