#include "type/maybe.h"

#include <cassert>

#include "json.h"

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace TypeChecker {

class Context;

Maybe::Maybe(yy::location loc, shared_ptr<Type> type)
    : Type(loc)
    , type_{move(type)} {
    assert(type_);
}

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
    type_ = TypeChecker::Type::verify(type_, ctx);
}

void
Maybe::pretty(ostream &out, bool) const {
    type_->pretty(out, true);
    out << "?";
}

bool
Maybe::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Maybe::isSuperImpl(const class Maybe &other, const Context &ctx) const {
    return other.type_->isSubtype(*type_, ctx);
}

shared_ptr<Type>
Maybe::simplify(Context &ctx) {
    type_ = type_->simplify(ctx);
    return shared_from_this();
}

shared_ptr<Type>
Maybe::callAsFunc(const Type &arg, const AST::Expression &call, Context &ctx) {
    auto type = type_->callAsFunc(arg, call, ctx);
    return make_shared<Maybe>(getLoc(), type);
}

shared_ptr<Type>
Maybe::indexAsArray(AST::Expression &arg, const AST::Expression &index, Context &ctx) {
    auto type = type_->indexAsArray(arg, index, ctx);
    return make_shared<Maybe>(getLoc(), type);
}

shared_ptr<Type>
Maybe::accessField(const string &field, const AST::Expression &access, Context &ctx) {
    auto type = type_->accessField(field, access, ctx);
    return make_shared<Maybe>(getLoc(), type);
}

} // namespace TypeChecker
