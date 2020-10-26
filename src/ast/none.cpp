#include "ast/none.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

None::None(const yy::location &loc)
    : Expression(loc) {}

void
None::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "none");
}
