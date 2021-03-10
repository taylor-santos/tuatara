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

using std::make_shared;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

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

shared_ptr<TypeChecker::Type>
Variable::getTypeImpl(TypeChecker::Context &ctx) {
    auto optType = ctx.getSymbol(name_);
    if (!optType) {
        throw TypeChecker::TypeException(
            "use of unidentified variable \"" + name_ + "\"",
            getLoc());
    }
    auto type = *optType;
    if (!type->isInitialized()) {
        vector<pair<string, yy::location>> msgs;
        msgs.emplace_back("use of uninitialized variable \"" + name_ + "\"", getLoc());
        msgs.emplace_back(
            "variable \"" + name_ + "\" was not initialized when it was declared",
            type->getLoc());
        throw TypeChecker::TypeException(msgs);
    }
    return type;
}

} // namespace AST
