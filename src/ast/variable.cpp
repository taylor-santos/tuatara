#include "ast/variable.h"

#include "type/type.h"
#include "type/type_context.h"
#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::ostream, std::pair, std::string, std::vector;

namespace AST {

Variable::Variable(const yy::location &loc, string name)
    : LValue(loc)
    , name_{move(name)} {}

Variable::~Variable() = default;

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
    if (!symbol) {
        throw TypeChecker::TypeException(
            "error: use of unidentified variable \"" + name_ + "\"",
            getLoc());
    }
    if (!symbol->initialized) {
        vector<pair<string, yy::location>> msgs;
        msgs.emplace_back("error: use of uninitialized variable \"" + name_ + "\"", getLoc());
        msgs.emplace_back(
            "note: \"" + name_ + "\" was not initialized when it was declared",
            symbol->type.getLoc());
        throw TypeChecker::TypeException(msgs);
    }
    return symbol->type;
}

} // namespace AST
