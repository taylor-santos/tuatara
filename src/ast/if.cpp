#include "ast/if.h"
#include "json.h"

using namespace AST;
using namespace std;

If::If(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt)
    : Statement(loc)
    , cond{move(cond)}
    , stmt{move(stmt)} {}

If::If(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt, Statement::Ptr else_stmt)
    : Statement(loc)
    , cond{move(cond)}
    , stmt{move(stmt)}
    , else_stmt{move(else_stmt)} {}

void
If::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "if");
    obj.Key("cond");
    os << *cond;
    obj.Key("statement");
    os << *stmt;
    if (else_stmt.has_value()) {
        obj.Key("else");
        os << **else_stmt;
    }
}
