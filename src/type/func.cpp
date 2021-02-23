#include "type/func.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Func::Func(yy::location loc, Type::Ptr argType, Type::Ptr retType)
    : Type(loc)
    , argType_{move(argType)}
    , retType_{move(retType)} {}

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "func");
    obj.printKeyValue("arg", argType_);
    obj.printKeyValue("return type", retType_);
}

void
Func::walk(const std::function<void(const Node &)> &fn) const {
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

bool
Func::operator<=(const Type &other) const {
    return other >= (*this);
}

bool
Func::operator>=(const Func &other) const {
    return (*argType_) <= (*other.argType_) && (*other.retType_) <= (*retType_);
}

} // namespace TypeChecker
