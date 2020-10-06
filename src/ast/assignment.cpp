#include "ast/assignment.h"

#include <utility>
#include "json.h"

using namespace TuataraAST;
using namespace std;

Assignment::Assignment(
    const yy::location &        loc,
    std::string                 variable,
    std::unique_ptr<Expression> value)
    : Expression(loc)
    , variable{move(variable)}
    , value{move(value)} {}

void
Assignment::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "assignment");
    obj.KeyValue("variable", variable);
    obj.Key("value");
    os << *value;
}
