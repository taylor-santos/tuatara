#include "ast/variable.h"
#include "json.h"

using namespace std;

namespace AST {

Variable::Variable(const yy::location &loc, string name)
    : LValue(loc)
    , name{move(name)} {}

void
Variable::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "variable");
    obj.KeyValue("name", name);
}

} // namespace AST