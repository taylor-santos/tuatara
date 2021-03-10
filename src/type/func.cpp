#include "type/func.h"

#include <cassert>
#include <sstream>

#include "ast/call.h"
#include "ast/expression.h"

#include "type/type_exception.h"

#include "json.h"

using std::function;
using std::make_shared;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace TypeChecker {

Func::Func(yy::location loc, shared_ptr<Type> argType, shared_ptr<Type> retType)
    : Type(loc)
    , argType_{move(argType)}
    , retType_{move(retType)} {
    assert(argType_);
    assert(retType_);
}

Func::~Func() = default;

shared_ptr<Type>
Func::clone(const yy::location &loc) const {
    return make_shared<Func>(
        loc,
        argType_->clone(argType_->getLoc()),
        retType_->clone(retType_->getLoc()));
}

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "func");
    obj.printKeyValue("arg", argType_);
    obj.printKeyValue("return type", retType_);
}

void
Func::walk(const function<void(const Node &)> &fn) const {
    Type::walk(fn);
    argType_->walk(fn);
    retType_->walk(fn);
}

const string &
Func::getNodeName() const {
    const static string name = "Func Type";
    return name;
}

void
Func::verifyImpl(Context &ctx) {
    argType_ = argType_->verify(ctx);
    retType_ = retType_->verify(ctx);
}

void
Func::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    argType_->pretty(out, true);
    out << "->";
    retType_->pretty(out, true);
    if (mod) {
        out << ")";
    }
}

shared_ptr<Type>
Func::simplify(Context &ctx) {
    argType_ = argType_->simplify(ctx);
    retType_ = retType_->simplify(ctx);
    return shared_from_this();
}

std::shared_ptr<Type>
Func::callAsFunc(const Type &arg, const yy::location &loc, Context &ctx) {
    if (arg.isSubtype(*argType_, ctx)) {
        return retType_->clone(loc);
    }
    vector<pair<string, yy::location>> msgs;
    {
        stringstream ss;
        ss << "function \"";
        TypeChecker::Type::pretty(ss);
        ss << "\" expects an argument with type \"";
        argType_->pretty(ss);
        ss << "\" but got \"";
        arg.pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), loc);
    }
    arg.addTypeLocMessage(msgs, "argument");
    addTypeLocMessage(msgs, "function");
    throw TypeException(msgs);
}

bool
Func::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Func::isSuperImpl(const class Func &other, const Context &ctx) const {
    return argType_->isSubtype(*other.argType_, ctx) && other.retType_->isSubtype(*retType_, ctx);
}

void
Func::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

} // namespace TypeChecker
