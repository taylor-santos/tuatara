#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/wildcard.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, WildcardJSON) {
    stringstream ss;
    yy::location loc;
    Wildcard     node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"pattern":"wildcard"})");
}

TEST(PatternTest, WildcardWalk) {
    stringstream ss;
    yy::location loc;
    Wildcard     node(loc);
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Wildcard Pattern\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
