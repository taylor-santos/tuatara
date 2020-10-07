#include "ast/float.h"
#include "json.h"

#include <utility>

using namespace TuataraAST;
using namespace std;

Float::Float(const yy::location &loc, double value)
    : Expression(loc)
    , value{value} {}

void
Float::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "float");
    obj.KeyValue("value", value);
}
