#include "ast/match.h"

#include "pattern/pattern.h"

#include "type/type_exception.h"

#include "json.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

using std::function;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

namespace AST {

Match::Match(const yy::location &loc, unique_ptr<Expression> value, vector<Case> cases)
    : Expression(loc)
    , value_{move(value)}
    , cases_{move(cases)} {}

Match::~Match() = default;

void
Match::json(ostream &os) const {
    JSON::Object obj(os);
    obj.printKeyValue("node", "match");
    obj.printKeyValue("value", value_);
    obj.printKey("cases");
    JSON::Array arr(os);
    for (const auto &[pattern, body] : cases_) {
        arr.next();
        JSON::Object caseObj(os);
        caseObj.printKeyValue("pattern", pattern);
        caseObj.printKeyValue("body", body);
    }
}

void
Match::walk(const function<void(const Node &)> &fn) const {
    Expression::walk(fn);
    value_->walk(fn);
    for (const auto &[pattern, body] : cases_) {
        pattern->walk(fn);
        body->walk(fn);
    }
}

const string &
Match::getNodeName() const {
    const static string name = "Match";
    return name;
}

shared_ptr<TypeChecker::Type>
Match::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented (" LOC_STR ")",
        getLoc());
}

} // namespace AST
