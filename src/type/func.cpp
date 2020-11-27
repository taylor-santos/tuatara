#include "type/func.h"
#include "json.h"

#include <utility>

using namespace TypeChecker;
using namespace std;

Func::Func(yy::location loc, optional<Type::Ptr> arg_type, optional<Type::Ptr> ret_type)
    : Type(loc)
    , arg_type{move(arg_type)}
    , ret_type{move(ret_type)} {}

void
Func::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("kind", "func");
    if (arg_type) {
        obj.KeyValue("arg", **arg_type);
    }
    if (ret_type) {
        obj.KeyValue("returns", **ret_type);
    }
}
