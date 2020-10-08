#include "ast/variable.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

Variable::Variable(const yy::location &loc, string name)
    : LValue(loc)
    , name{move(name)} {}

void
Variable::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "variable");
    obj.KeyValue("name", name);
}
