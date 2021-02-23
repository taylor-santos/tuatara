#include "ast/match.h"

#include <algorithm>
#include <array>

#include "type/type_exception.h"

#include "json.h"

using namespace std;

namespace AST {

Match::Match(const yy::location &loc, Expression::Ptr value, std::vector<Case> cases)
    : Expression(loc)
    , value_{move(value)}
    , cases_{move(cases)} {}

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
    for_each(cases_.begin(), cases_.end(), [&](const auto &patternAndBody) {
        patternAndBody.first->walk(fn);
        patternAndBody.second->walk(fn);
    });
}

const string &
Match::getNodeName() const {
    const static string name = "Match";
    return name;
}

TypeChecker::Type &
Match::getTypeImpl(TypeChecker::Context &) {
    throw TypeChecker::TypeException(
        "type error: " + getNodeName() + " type checking not implemented",
        getLoc());
}

} // namespace AST
