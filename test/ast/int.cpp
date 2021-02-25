#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/int.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IntJSON) {
    ostringstream ss;
    yy::location  loc;
    Int           node(loc, 123);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"int","value":123})");
}

TEST(ASTTest, IntWalk) {
    ostringstream ss;
    yy::location  loc;
    Int           node(loc, 123);
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Int\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
