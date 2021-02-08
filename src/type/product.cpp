#include "type/product.h"
#include "json.h"

using namespace std;

namespace TypeChecker {

Product::Product(yy::location loc, vector<Type::Ptr> types)
    : Type(loc)
    , types{move(types)} {}

void
Product::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "product");
    obj.Key("types");
    JSON::Array arr(os);
    for (const auto &type : types) {
        arr.Next();
        os << *type;
    }
}

} // namespace TypeChecker
