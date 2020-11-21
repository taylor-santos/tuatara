#include "ast/return.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

Return::Return(const yy::location &loc)
    : Statement(loc) {}

Return::Return(const yy::location &loc, std::unique_ptr<Expression> value)
    : Statement(loc)
    , value{move(value)} {}

void
Return::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "return");
    if (value) {
        obj.KeyValue("returns", **value);
    }
}
