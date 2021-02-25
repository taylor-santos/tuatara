#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/index.h"

#include "ast/int.h"
#include "ast/variable.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IndexJSON) {
    ostringstream ss;
    yy::location  loc;
    auto          expr  = make_unique<Variable>(loc, "a");
    auto          index = make_unique<Int>(loc, 5);
    Index         node(loc, move(expr), move(index));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"array index",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("index":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ASTTest, IndexWalk) {
    ostringstream ss;
    yy::location  loc;
    auto          expr  = make_unique<Variable>(loc, "a");
    auto          index = make_unique<Int>(loc, 5);
    Index         node(loc, move(expr), move(index));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Index\nVariable\nInt\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
