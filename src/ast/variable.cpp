#include "ast/variable.h"

#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using namespace std;

namespace AST {

Variable::Variable(const yy::location &loc, string name)
    : LValue(loc)
    , name_{move(name)} {}

void
Variable::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "variable");
    obj.printKeyValue("name", name_);
}

const string &
Variable::getNodeName() const {
    const static string name = "Variable";
    return name;
}

TypeChecker::Type &
Variable::getTypeImpl(TypeChecker::Context &ctx) {
    auto symbol = ctx.getSymbol(name_);
    if (symbol && symbol->initialized) {
        return symbol->type;
    }
    throw TypeChecker::TypeException(
        "error: variable \"" + name_ + "\" used before initialization",
        getLoc());
}

} // namespace AST
