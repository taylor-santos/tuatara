#include "ast/while.h"
#include "json.h"

using namespace AST;
using namespace std;

While::While(const yy::location &loc, std::unique_ptr<Expression> cond, unique_ptr<Statement> stmt)
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