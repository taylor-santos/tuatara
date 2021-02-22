#include "ast/variable.h"

#include "json.h"

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
Variable::getTypeName() const {
    const static string name = "Variable";
    return name;
}

} // namespace AST
