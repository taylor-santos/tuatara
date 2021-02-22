#include "ast/match.h"

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
Match::walk(const Func &fn) const {
    Expression::walk(fn);
    value_->walk(fn);
    for (const auto &[pattern, body] : cases_) {
        pattern->walk(fn);
        body->walk(fn);
    }
}

const string &
Match::getTypeName() const {
    const static string name = "Match";
    return name;
}

} // namespace AST
