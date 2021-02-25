#include "type/type.h"

#include <sstream>

#include "type/type_exception.h"

using std::ostream, std::stringstream;

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
    verifyState_ = VerifyState::VERIFIED;
}

void
Type::pretty(ostream &out) const {
    pretty(out, false);
}

Type &
Type::callAsFunc(Context &, AST::Expression &) {
    stringstream ss;
    ss << "error: \"";
    pretty(ss);
    ss << "\" cannot be called as a function";
    throw TypeException(ss.str(), getLoc());
}

bool
Type::isSupertype(const class Array &) const {
    return false;
}

bool
Type::isSupertype(const class Class &) const {
    return false;
}

bool
Type::isSupertype(const class Func &) const {
    return false;
}

bool
Type::isSupertype(const class Maybe &) const {
    return false;
}

bool
Type::isSupertype(const class Object &) const {
    return false;
}

bool
Type::isSupertype(const class Product &) const {
    return false;
}

bool
Type::isSupertype(const class Sum &) const {
    return false;
}

bool
Type::isSupertype(const class Unit &) const {
    return false;
}

} // namespace TypeChecker
