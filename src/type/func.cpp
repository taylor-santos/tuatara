#include "type/func.h"

#include <common.h>

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
    , retType_{move(retType)} {}

Func::~Func() = default;

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
    argType_->verify(ctx);
    retType_->verify(ctx);
}

void
Func::pretty(ostream &out, bool mod) const {
    if (mod) {
        out << "(";
    }
    argType_->pretty(out, false);
    out << "->";
    retType_->pretty(out, false);
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
Func::callAsFunc(Context &ctx, AST::Expression &arg, const AST::Call &call) {
    auto type = arg.getType(ctx);
    if (type->isSubtype(*argType_, ctx)) {
        return retType_;
    }
    vector<pair<string, yy::location>> msgs;
    {
        stringstream ss;
        ss << "error: function expecting an argument with type \"";
        argType_->pretty(ss);
        ss << "\" cannot be called with type \"";
        type->pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), arg.getLoc());
    }
    msgs.emplace_back("note: function invoked here", call.getLoc());
    {
        stringstream ss;
        ss << "note: function has signature \"";
        Type::pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), getLoc());
    }
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

} // namespace TypeChecker
