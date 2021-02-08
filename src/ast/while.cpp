#include "ast/while.h"
#include "json.h"

using namespace std;

namespace AST {

While::While(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt)
    : Statement(loc)
    , cond{move(cond)}
    , stmt{move(stmt)} {}

void
While::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "while");
    obj.Key("cond");
    os << *cond;
    obj.Key("statement");
    os << *stmt;
}

} // namespace AST