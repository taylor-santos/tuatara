#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/tuple.h"

#include "ast/int.h"
#include "ast/variable.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, TupleJSON) {
    ostringstream                  ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> exprs;
    exprs.emplace_back(make_unique<Variable>(loc, "var"));
    exprs.emplace_back(make_unique<Int>(loc, 123));
    Tuple node(loc, move(exprs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"tuple",)"
        R"("expressions":[{)"
        R"("node":"variable",)"
        R"("name":"var"},{)"
        R"("node":"int",)"
        R"("value":123}]})");
}

TEST(ASTTest, TupleWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> exprs;
    exprs.emplace_back(make_unique<Variable>(loc, "var"));
    exprs.emplace_back(make_unique<Int>(loc, 123));
    Tuple node(loc, move(exprs));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Tuple\nVariable\nInt\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif