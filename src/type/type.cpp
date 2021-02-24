#include "type/type.h"

#include <sstream>

#include "ast/expression.h"

#include "type/type_exception.h"

using namespace std;

namespace TypeChecker {

Type::Type(yy::location loc)
    : Node(loc) {}

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
Type::pretty(std::ostream &out) const {
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
Type::operator>=(const Type &) const {
    return false;
}

bool
Type::operator>=(const class Array &) const {
    return false;
}

bool
Type::operator>=(const class Class &) const {
    return false;
}

bool
Type::operator>=(const class Func &) const {
    return false;
}

bool
Type::operator>=(const class Maybe &) const {
    return false;
}

bool
Type::operator>=(const class Object &) const {
    return false;
}

bool
Type::operator>=(const class Product &) const {
    return false;
}

bool
Type::operator>=(const class Sum &) const {
    return false;
}

bool
Type::operator>=(const class Unit &) const {
    return false;
}

} // namespace TypeChecker
