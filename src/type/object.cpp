#include "type/object.h"

#include <sstream>

#include "type/class.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

using std::ostream;
using std::string;
using std::stringstream;

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
    auto symbol = ctx.getClass(className_);
    if (!symbol) {
        throw TypeException({"error: unknown class name \"" + className_ + "\"", getLoc()});
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

} // namespace TypeChecker
