#include "type/array.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Array::Array(yy::location loc, Type::Ptr type)
    : Type(loc)
    , type_{move(type)} {}

void
Array::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "array");
    obj.printKeyValue("type", type_);
}

void
Array::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
    type_->walk(fn);
}

const string &
Array::getTypeName() const {
    const static string name = "Array Type";
    return name;
}

} // namespace TypeChecker
