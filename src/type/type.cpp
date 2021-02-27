#include "type/type.h"

#include <sstream>

#include "type/type_exception.h"

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::stringstream;

namespace TypeChecker {

Type::Type(yy::location loc)
    : Node(loc) {}

Type::~Type() = default;

void
Type::verify(Context &ctx) {
    if (verifyState_ != VerifyState::NONE) {
        return;
    }
    verifyState_ = VerifyState::FAILED;
    verifyImpl(ctx); // Throws on failure
    simplify(ctx);
    verifyState_ = VerifyState::VERIFIED;
}

void
Type::pretty(ostream &out) const {
    pretty(out, false);
}

shared_ptr<Type>
Type::callAsFunc(Context &, AST::Expression &) {
    stringstream ss;
    ss << "error: \"";
    pretty(ss);
    ss << "\" cannot be called as a function";
    throw TypeException(ss.str(), getLoc());
}

bool
Type::isSupertype(const Type &other, Context &ctx) const {
    return other.isSubtype(*this, ctx);
}

bool
Type::isEqual(const Type &other, Context &ctx) const {
    if (this == &other) {
        return true;
    }
    return this->isSupertype(other, ctx) && this->isSubtype(other, ctx);
}

std::shared_ptr<Type>
Type::simplify(Context &) {
    return shared_from_this();
}

bool
Type::isSuperImpl(const class Array &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Class &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Func &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Maybe &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Object &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Product &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Sum &, Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Unit &, Context &) const {
    return false;
}

void
Type::setVerifyState(Type::VerifyState verifyState) {
    verifyState_ = verifyState;
}

bool
Type::isInitialized() const {
    return initialized_;
}

void
Type::setInitialized(bool initialized) {
    initialized_ = initialized;
}

} // namespace TypeChecker
