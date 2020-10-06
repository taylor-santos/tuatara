#include "ast/int.h"
#include "json.h"

#include <utility>

using namespace TuataraAST;
using namespace std;

Int::Int(const yy::location &loc, int value)
    : Expression(loc)
    , value{value} {}

void
Int::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "int");
    obj.KeyValue("value", value);
}