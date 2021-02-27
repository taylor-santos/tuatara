#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/unit.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, UnitJSON) {
    ostringstream ss;
    yy::location  loc;
    Unit          node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"unit"})");
}

TEST(ASTTest, UnitWalk) {
    ostringstream ss;
    yy::location  loc;
    Unit          node(loc);
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Unit\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif