#include "ast/assignment.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace std;

Assignment::Assignment(const yy::location &loc, unique_ptr<LValue> lhs, unique_ptr<Expression> rhs)
    : Expression(loc)
    , lhs{move(lhs)}
    , rhs{move(rhs)} {}

void
Assignment::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "assignment");
    obj.Key("lhs");
    os << *lhs;
    obj.Key("rhs");
    os << *rhs;
}
