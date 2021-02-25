#include "type/func.h"

#include <sstream>

#include "ast/expression.h"

#include "type/type_exception.h"

#include "json.h"

using std::function, std::ostream, std::pair, std::string, std::stringstream, std::unique_ptr,
    std::vector;

namespace TypeChecker {

Func::Func(yy::location loc, unique_ptr<Type> argType, unique_ptr<Type> retType)
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
    out << "func(";
    argType_->pretty(out, false);
    out << "):";
    retType_->pretty(out, false);
    if (mod) {
        out << ")";
    }
}

Type &
Func::callAsFunc(Context &ctx, AST::Expression &arg) {
    auto &type = arg.getType(ctx);
    if (type.isSubtype(*argType_, ctx)) {
        return *retType_;
    }
    vector<pair<string, yy::location>> msgs;
    {
        stringstream ss;
        ss << "error: function expecting an argument with type \"";
        argType_->pretty(ss);
        ss << "\" cannot be called with type \"";
        type.pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), arg.getLoc());
    }
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
Func::isSubtype(const Type &other, Context &ctx) const {
    return other.isSupertype(*this, ctx);
}

bool
Func::isSupertype(const Type &other, Context &ctx) const {
    return other.isSubtype(*this, ctx);
}

bool
Func::isSupertype(const class Func &other, Context &ctx) const {
    return argType_->isSubtype(*other.argType_, ctx) && other.retType_->isSubtype(*retType_, ctx);
}

} // namespace TypeChecker
