#include "ast/assignment.h"

#include <utility>
#include "json.h"

using namespace AST;
using namespace std;

Assignment::Assignment(const yy::location &loc, string variable, unique_ptr<Expression> value)
    : Expression(loc)
    , variable{move(variable)}
    , value{move(value)} {}

void
Assignment::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "assignment");
    obj.KeyValue("variable", variable);
    obj.Key("value");
    os << *value;
}
