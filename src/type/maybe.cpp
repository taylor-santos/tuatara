#include "type/maybe.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Maybe::Maybe(yy::location loc, Type::Ptr type)
    : Type(loc)
    , type_{move(type)} {}

void
Maybe::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "maybe");
    obj.printKeyValue("type", type_);
}

void
Maybe::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
    type_->walk(fn);
}

const string &
Maybe::getTypeName() const {
    const static string name = "Maybe Type";
    return name;
}

} // namespace TypeChecker
