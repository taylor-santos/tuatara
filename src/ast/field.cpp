#include "ast/field.h"

#include <cassert>
#include <sstream>

#include "type/class.h"
#include "type/object.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker

using std::dynamic_pointer_cast;
using std::function;
using std::make_shared;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

namespace AST {

Field::Field(
    const yy::location &   loc,
    unique_ptr<Expression> expr,
    const yy::location &   fieldLoc,
    string                 field)
    : LValue(loc)
    , expr_{move(expr)}
    , fieldLoc_{fieldLoc}
    , field_{move(field)} {
    assert(expr_);
}

Field::~Field() = default;

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

shared_ptr<TypeChecker::Type>
Field::getTypeImpl(TypeChecker::Context &ctx) {
    auto type = expr_->getType(ctx);
    return type->accessField(field_, *this, ctx);
}

} // namespace AST
