#include "type/object.h"

#include <sstream>

#include "type/class.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

using std::ostream, std::string, std::stringstream;

namespace TypeChecker {

Object::Object(yy::location loc, string className)
    : Type(loc)
    , className_{move(className)} {}

Object::~Object() = default;

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
    auto symbol = ctx.getSymbol(className_);
    if (!symbol) {
        throw TypeException({"error: unknown class name \"" + className_ + "\"", getLoc()});
    }
    auto cl = dynamic_cast<TypeChecker::Class *>(&symbol->type);
    if (!cl) {
        stringstream ss;
        ss << "note: \"" << className_ << "\" previously defined as \"";
        symbol->type.pretty(ss);
        ss << "\"";
        throw TypeException(
            {{"error: type name \"" + className_ + "\" does not name a class.", getLoc()},
             {ss.str(), symbol->type.getLoc()}});
    }
    ofClass_ = *cl;
}

void
Object::pretty(ostream &out, bool) const {
    out << className_;
}

bool
Object::isSubtype(const Type &other) const {
    return other.isSupertype(*this);
}

bool
Object::isSupertype(const Type &other) const {
    return other.isSubtype(*this);
}

bool
Object::isSupertype(const Object &other) const {
    return other.ofClass_->get().isSubtype(ofClass_->get());
}

const Class &
Object::getClass() const {
    return *ofClass_;
}

} // namespace TypeChecker
