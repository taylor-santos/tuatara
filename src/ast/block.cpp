#include "ast/block.h"
#include "json.h"

using namespace std;

namespace AST {

Block::Block(const yy::location &loc, Statement::Vec stmts)
    : Expression(loc)
    , stmts{move(stmts)} {}

void
Block::json(ostream &os) const {
    JSON::Object obj(os);
    obj.KeyValue("node", "block");
    obj.Key("statements");
    JSON::Array(os) << stmts;
}

} // namespace AST
