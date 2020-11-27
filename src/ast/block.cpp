#include "ast/block.h"
#include "json.h"

using namespace AST;
using namespace std;

Block::Block(const yy::location &loc, Statement::Vec stmts)
    : Expression(loc)
    , stmts{move(stmts)} {}

void
Block::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "block");
    obj.Key("statements");
    JSON::Array arr(os);
    for (const auto &stmt : stmts) {
        arr.Next();
        os << *stmt;
    }
}
