#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/match.h"

#include "ast/int.h"
#include "ast/variable.h"

#include "pattern/literal.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, MatchJSON) {
    ostringstream       ss;
    yy::location        loc;
    vector<Match::Case> cases;
    auto                val  = make_unique<Int>(loc, 5);
    auto                pat  = make_unique<Pattern::Literal>(loc, move(val));
    auto                expr = make_unique<Variable>(loc, "bar");
    cases.emplace_back(make_pair(move(pat), move(expr)));
    Match node(loc, make_unique<Variable>(loc, "foo"), move(cases));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"match",)"
        R"("value":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("cases":[{)"
        R"("pattern":{)"
        R"("pattern":"literal",)"
        R"("literal":{)"
        R"("node":"int",)"
        R"("value":5}},)"
        R"("body":{)"
        R"("node":"variable",)"
        R"("name":"bar"}}]})");
}

TEST(ASTTest, MatchWalk) {
    ostringstream       ss;
    yy::location        loc;
    vector<Match::Case> cases;
    auto                val  = make_unique<Int>(loc, 5);
    auto                pat  = make_unique<Pattern::Literal>(loc, move(val));
    auto                expr = make_unique<Variable>(loc, "bar");
    cases.emplace_back(make_pair(move(pat), move(expr)));
    Match node(loc, make_unique<Variable>(loc, "foo"), move(cases));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Match\nVariable\nLiteral Pattern\nInt\nVariable\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
