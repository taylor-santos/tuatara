#include "type/sum.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Sum::Sum(yy::location loc, Type::Vec types)
    : Type(loc)
    , types_{move(types)} {}

void
Sum::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "sum");
    obj.printKeyValue("types", types_);
}

void
Sum::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
    for (const auto &type : types_) {
        type->walk(fn);
    }
}

const string &
Sum::getTypeName() const {
    const static string name = "Sum Type";
    return name;
}

} // namespace TypeChecker
