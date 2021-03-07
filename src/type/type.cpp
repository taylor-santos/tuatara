#include "type/type.h"

#include <sstream>

#include "ast/expression.h"

#include "type/type_exception.h"

using std::make_shared;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

namespace TypeChecker {

Type::Type(yy::location loc)
    : Node(loc) {}

Type::~Type() = default;

std::shared_ptr<Type>
Type::verify(std::shared_ptr<Type> type, Context &ctx) {
    if (type->verifyState_ != VerifyState::NONE) {
        return type;
    }
    type->verifyState_ = VerifyState::FAILED;
    type->verifyImpl(ctx); // Throws on failure
    type               = type->simplify(ctx);
    type->verifyState_ = VerifyState::VERIFIED;
    return type;
}

void
Type::pretty(ostream &out) const {
    pretty(out, false);
}

std::shared_ptr<Type>
Type::callAsFunc(const Type &, const AST::Expression &call, Context &) {
    vector<pair<string, yy::location>> msgs;
    msgs.emplace_back("error: expression is not a function", call.getLoc());
    addTypeLocMessage(msgs, "expression");
    throw TypeException(msgs);
}

std::shared_ptr<Type>
Type::indexAsArray(AST::Expression &, const AST::Expression &index, Context &) {
    vector<pair<string, yy::location>> msgs;
    msgs.emplace_back("error: expression cannot be indexed", index.getLoc());
    addTypeLocMessage(msgs, "expression");
    throw TypeException(msgs);
}

std::shared_ptr<Type>
Type::accessField(const std::string &field, const AST::Expression &access, Context &) {
    vector<pair<string, yy::location>> msgs;
    stringstream                       ss;
    ss << "error: cannot access member \"" << field << "\" of non-class type";
    msgs.emplace_back(ss.str(), access.getLoc());
    addTypeLocMessage(msgs, "expression");
    throw TypeChecker::TypeException(msgs);
}

bool
Type::isSupertype(const Type &other, const Context &ctx) const {
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
Type::isSuperImpl(const class Array &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Class &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Func &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Maybe &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Object &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Product &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Sum &, const Context &) const {
    return false;
}

bool
Type::isSuperImpl(const class Unit &, const Context &) const {
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

bool
Type::addTypeLocMessage(vector<pair<string, yy::location>> &msgs, const string &name) const {
    auto loc = getLoc();
    if (loc.begin.line == 0) {
        return false;
    }
    stringstream ss;
    if (name.empty()) {
        ss << "node: given type \"";
    } else {
        ss << "note: " << name << " given type \"";
    }
    pretty(ss);
    ss << "\" here:";
    msgs.emplace_back(ss.str(), loc);
    return true;
}

} // namespace TypeChecker
