#include "ast/ident_access.h"

#include <sstream>
#include <string>

#include "ast/call.h"
#include "ast/field.h"
#include "ast/variable.h"

#include "type/func.h"
#include "type/object.h"
#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

IdentAccess::IdentAccess(
    const yy::location &loc,
    Expression::Ptr     expr,
    const yy::location &idLoc,
    string              ident)
    : LValue(loc)
    , expr_{move(expr)}
    , idLoc_{idLoc}
    , ident_{move(ident)} {}

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
    LValue::walk(fn);
    expr_->walk(fn);
}

const string &
IdentAccess::getNodeName() const {
    const static string name = "Ident Access";
    return name;
}

TypeChecker::Type &
IdentAccess::getTypeImpl(TypeChecker::Context &ctx) {
    /* The expression "a b" can be interpreted in two different ways depending on the type of a:
     *  1) If a is an object, "a b" is equivalent to a member access expression "a.b".
     *  2) If a is a function, "a b" is equivalent to the function call "a(b)".
     */
    auto &type = expr_->getType(ctx);
    auto  obj  = dynamic_cast<TypeChecker::Object *>(&type);
    if (obj) {
        state_ = make_unique<Field>(getLoc(), move(expr_), idLoc_, ident_);
        return (*state_)->getType(ctx);
    }
    auto arg = make_unique<Variable>(idLoc_, ident_);
    state_   = make_unique<Call>(getLoc(), move(expr_), move(arg));
    return (*state_)->getType(ctx);
}

} // namespace AST
