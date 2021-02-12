#include "ast/stub.h"

#include "json.h"

using namespace std;

namespace AST {

Stub::Stub(const yy::location &loc, std::string data)
    : Node(loc)
    , data_{move(data)} {}

void
Stub::json(std::ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "stub");
    obj.printKeyValue("data", data_);
}

void
Stub::walk(const Node::Func &fn) const {
    Node::walk(fn);
}

const std::string &
Stub::getTypeName() const {
    const static string name = "Stub";
    return name;
}

} // namespace AST
