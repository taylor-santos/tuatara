#include "type/object.h"

#include <sstream>

#include "ast/expression.h"

#include "type/class.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

using std::make_shared;
using std::ostream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

namespace TypeChecker {

Object::Object(yy::location loc, string className)
    : Type(loc)
    , className_{move(className)} {}

Object::~Object() = default;

std::shared_ptr<Type>
Object::clone(const yy::location &loc) const {
    return make_shared<Object>(loc, className_);
}

void
Object::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "object");
    obj.printKeyValue("class", className_);
}
const string &
Object::getNodeName() const {
    const static string name = "Object Type";
    return name;
}

void
Object::verifyImpl(Context &ctx) {
    auto symbol = ctx.getClass(className_);
    if (!symbol) {
        throw TypeException({"unknown class name \"" + className_ + "\"", getLoc()});
    }
    ofClass_ = *symbol;
}

void
Object::pretty(ostream &out, bool) const {
    out << className_;
}

bool
Object::isSubtype(const Type &other, const Context &ctx) const {
    return other.isSuperImpl(*this, ctx);
}

bool
Object::isSuperImpl(const class Object &other, const Context &ctx) const {
    return other.ofClass_->get().isSubtype(ofClass_->get(), ctx);
}

const Class &
Object::getClass() const {
    return *ofClass_;
}

std::shared_ptr<Type>
Object::accessField(const string &field, const yy::location &loc, Context &) {
    auto optField = (*ofClass_).get().getField(field);
    if (!optField) {
        vector<pair<string, yy::location>> msgs;
        {
            stringstream ss;
            ss << "no member named \"" << field << "\" in object with type \""
               << (*ofClass_).get().getClassName() << "\"";
            msgs.emplace_back(ss.str(), loc);
        }
        addTypeLocMessage(msgs, "object");
        throw TypeChecker::TypeException(msgs);
    }
    return *optField;
}

void
Object::updateWith(const Type &other) {
    other.updateForImpl(*this);
}

} // namespace TypeChecker
