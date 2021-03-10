#include "ast/ident_access.h"

#include <cassert>
#include <sstream>

#include "ast/call.h"
#include "ast/field.h"
#include "ast/variable.h"

#include "type/object.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::dynamic_pointer_cast;
using std::function;
using std::make_shared;
using std::make_unique;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

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
    JSON::Object obj(os);
    obj.printKeyValue("node", "ident access");
    obj.printKeyValue("expr", expr_);
    obj.printKeyValue("ident", ident_);
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

shared_ptr<TypeChecker::Type>
IdentAccess::getTypeImpl(TypeChecker::Context &ctx) {
    /* The expression "a b" can be interpreted in two different ways depending on the type of a:
     *  1) If a is an object, "a b" is equivalent to a member access expression "a.b", in which case
     *     b would need to be the name of one of a's fields.
     *  2) If a is a function, "a b" is equivalent to the function call "a(b)", in which case b
     *     would need to resolve to a valid local variable compatible with the function a.
     */
    vector<pair<string, yy::location>> msgs;
    yy::location                       nullLoc{nullptr, 0, 0};
    auto                               type = expr_->getType(ctx);
    try {
        auto fieldType = type->accessField(ident_, expr_->getLoc(), ctx);
        return fieldType;
    } catch (const TypeChecker::TypeException &e) {
        stringstream ss;
        ss << "\"" << ident_ << "\" cannot be applied to an expression with type \"";
        type->pretty(ss);
        ss << "\"";
        msgs.emplace_back(ss.str(), getLoc());
        msgs.emplace_back("\"" + ident_ + "\" cannot be a field because:", nullLoc);
        msgs.insert(msgs.end(), e.getMsgs().begin(), e.getMsgs().end());
        msgs.emplace_back(
            "\"" + ident_ + "\" cannot be the argument to a function because:",
            nullLoc);
    }

    auto optIdentType = ctx.getSymbol(ident_);
    if (!optIdentType) {
        msgs.emplace_back("\"" + ident_ + "\" is not the name of a variable in scope", getLoc());
    } else if (!(*optIdentType)->isInitialized()) {
        msgs.emplace_back(
            "the variable \"" + ident_ + "\" has not been initialized",
            (*optIdentType)->getLoc());
    } else {
        try {
            auto funcRetType = type->callAsFunc(**optIdentType, getLoc(), ctx);
            return funcRetType;
        } catch (TypeChecker::TypeException &e) {
            const auto &newMsgs = e.getMsgs();
            msgs.insert(msgs.end(), newMsgs.begin(), newMsgs.end());
        }
    }
    throw TypeChecker::TypeException(msgs);
}

} // namespace AST
