#include "ast/if.h"
#include "json.h"

using namespace std;

namespace AST {

If::If(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt)
    : Statement(loc)
    , cond{move(cond)}
    , stmt{move(stmt)} {}

If::If(const yy::location &loc, Expression::Ptr cond, Statement::Ptr stmt, Statement::Ptr elseStmt)
    : Statement(loc)
    , cond{move(cond)}
    , stmt{move(stmt)}
    , elseStmt{move(elseStmt)} {}

void
If::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "if");
    obj.Key("cond");
    os << *cond;
    obj.Key("statement");
    os << *stmt;
    if (elseStmt.has_value()) {
        obj.Key("else");
        os << **elseStmt;
    }
}

} // namespace AST
