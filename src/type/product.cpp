#include "type/product.h"

#include "json.h"

using namespace std;

namespace TypeChecker {

Product::Product(yy::location loc, vector<Type::Ptr> types)
    : Type(loc)
    , types_{move(types)} {}

void
Product::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("kind", "product");
    obj.printKeyValue("types", types_);
}

void
Product::walk(const AST::Node::Func &fn) const {
    Type::walk(fn);
    for (const auto &type : types_) {
        type->walk(fn);
    }
}

const string &
Product::getTypeName() const {
    const static string name = "Product Type";
    return name;
}

} // namespace TypeChecker
