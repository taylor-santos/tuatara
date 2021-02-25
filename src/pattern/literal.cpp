#include "pattern/literal.h"

#include "ast/literal.h"

#include "json.h"

namespace yy {
class location;
} // namespace yy

using std::function, std::ostream, std::string, std::unique_ptr;

namespace Pattern {

Literal::Literal(const yy::location &loc, unique_ptr<AST::Literal> literal)
    : Pattern(loc)
    , literal_{move(literal)} {}

Literal::~Literal() = default;

void
Literal::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("pattern", "literal");
    obj.printKeyValue("literal", literal_);
}

void
Literal::walk(const function<void(const Node &)> &fn) const {
    Pattern::walk(fn);
    literal_->walk(fn);
}

const string &
Literal::getNodeName() const {
    const static string name = "Literal Pattern";
    return name;
}

TypeChecker::Type &
Literal::getTypeImpl(TypeChecker::Context &ctx) {
    return literal_->getType(ctx);
}

} // namespace Pattern
