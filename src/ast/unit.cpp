#include "ast/unit.h"

#include "type/unit.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::make_unique, std::ostream, std::string;

namespace AST {

Unit::Unit(const yy::location &loc)
    : LValue(loc)
    , myType_{make_unique<TypeChecker::Unit>(loc)} {}

Unit::~Unit() = default;

void
Unit::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "unit");
}

const string &
Unit::getNodeName() const {
    const static string name = "Unit";
    return name;
}

TypeChecker::Type &
Unit::getTypeImpl(TypeChecker::Context &) {
    return *myType_;
}

} // namespace AST
