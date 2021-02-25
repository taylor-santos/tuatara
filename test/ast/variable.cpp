#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/variable.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, VariableJSON) {
    ostringstream ss;
    yy::location  loc;
    Variable      node(loc, "var");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"variable","name":"var"})");
}

TEST(ASTTest, VariableWalk) {
    ostringstream ss;
    yy::location  loc;
    Variable      node(loc, "var");
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Variable\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
