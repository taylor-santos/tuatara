#include "ast/call.h"
#include "json.h"

#include <utility>

using namespace AST;
using namespace std;

Call::Call(const yy::location &loc, Expression::Ptr function, Expression::Vec args)
    : LValue(loc)
    , function{move(function)}
    , args{move(args)} {}

void
Call::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "function call");
    obj.KeyValue("function", *function);
    {
        obj.Key("args");
        JSON::Array arr(os);
        for (const auto &arg : args) {
            arr.Next();
            os << *arg;
        }
    }
}
