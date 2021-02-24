#include "ast/field.h"

#include <sstream>

#include "type/class.h"
#include "type/func.h"
#include "type/object.h"
#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

Field::Field(
    const yy::location &loc,
    Expression::Ptr     expr,
    const yy::location &fieldLoc,
    string              field)
    : LValue(loc)
    , expr_{move(expr)}
    , fieldLoc_{fieldLoc}
    , field_{move(field)} {}

void
Field::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "field");
    obj.printKeyValue("expr", expr_);
    obj.printKeyValue("field", field_);
}

void
Field::walk(const function<void(const Node &)> &fn) const {
    LValue::walk(fn);
    expr_->walk(fn);
}

const string &
Field::getNodeName() const {
    const static string name = "Field";
    return name;
}

TypeChecker::Type &
Field::getTypeImpl(TypeChecker::Context &ctx) {
    /* The expression "a b" can be interpreted in two different ways depending on the type of a:
     *  1) If a is an object, "a b" is equivalent to a field access expression "a.b".
     *  2) If a is a function, "a b" is equivalent to the function call "a(b)".
     */
    auto &type = expr_->getType(ctx);
    auto  obj  = dynamic_cast<TypeChecker::Object *>(&type);
    if (!obj) {
        stringstream ss;
        ss << "error: cannot access member of non-object type, is has type \"";
        type.pretty(ss);
        ss << "\"";
        throw TypeChecker::TypeException(ss.str(), expr_->getLoc());
    }
    const auto &ofClass = obj->getClass();
    auto *      field   = ofClass.getField(field_);
    if (!field) {
        stringstream ss;
        ss << "error: no member named \"" << field_ << "\" in class \"" << ofClass.getClassName()
           << "\"";
        throw TypeChecker::TypeException(ss.str(), fieldLoc_);
    }
    return *field;
}

} // namespace AST
