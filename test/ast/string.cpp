#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/string.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, StringJSON) {
    ostringstream ss;
    yy::location  loc;
    String        node(loc, "StringNode");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"string","value":"StringNode"})");
}

TEST(ASTTest, StringWalk) {
    ostringstream ss;
    yy::location  loc;
    String        node(loc, "StringNode");
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "String\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
