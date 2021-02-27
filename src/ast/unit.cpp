#include "ast/unit.h"

#include "type/unit.h"

#include "json.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::make_shared;
using std::make_unique;
using std::ostream;
using std::shared_ptr;
using std::string;

namespace AST {

Unit::Unit(const yy::location &loc)
    : LValue(loc) {}

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

shared_ptr<TypeChecker::Type>
Unit::getTypeImpl(TypeChecker::Context &) {
    return make_shared<TypeChecker::Unit>(getLoc());
}

} // namespace AST
