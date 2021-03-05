#include "ast/ident_access.h"

#include <cassert>

#include "ast/call.h"
#include "ast/field.h"
#include "ast/variable.h"

#include "type/object.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::dynamic_pointer_cast;
using std::function;
using std::make_shared;
using std::make_unique;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace AST {

IdentAccess::IdentAccess(
    const yy::location &   loc,
    unique_ptr<Expression> expr,
    const yy::location &   idLoc,
    string                 ident)
    : LValue(loc)
    , expr_{move(expr)}
    , idLoc_{idLoc}
    , ident_{move(ident)} {
    assert(expr_);
}

IdentAccess::~IdentAccess() = default;

void
IdentAccess::json(ostream &os) const {
    if (state_) {
        os << **state_;
    } else {
        JSON::Object obj(os);
        obj.printKeyValue("node", "ident access");
        obj.printKeyValue("expr", expr_);
        obj.printKeyValue("ident", ident_);
    }
}

void
IdentAccess::walk(const function<void(const Node &)> &fn) const {
    if (state_) {
        (*state_)->walk(fn);
    } else {
        LValue::walk(fn);
        expr_->walk(fn);
    }
}

const string &
IdentAccess::getNodeName() const {
    const static string name = "Ident Access";
    return name;
}

shared_ptr<TypeChecker::Type>
IdentAccess::getTypeImpl(TypeChecker::Context &ctx) {
    /* The expression "a b" can be interpreted in two different ways depending on the type of a:
     *  1) If a is an object, "a b" is equivalent to a member access expression "a.b".
     *  2) If a is a function, "a b" is equivalent to the function call "a(b)".
     */
    if (state_) {
        return (*state_)->getType(ctx);
    }
    auto type = expr_->getType(ctx);
    auto obj  = dynamic_pointer_cast<TypeChecker::Object>(type);
    if (obj) {
        state_ = make_unique<Field>(getLoc(), move(expr_), idLoc_, ident_);
        return (*state_)->getType(ctx);
    }
    auto arg = make_unique<Variable>(idLoc_, ident_);
    state_   = make_unique<Call>(getLoc(), move(expr_), move(arg));
    return (*state_)->getType(ctx);
}

} // namespace AST
