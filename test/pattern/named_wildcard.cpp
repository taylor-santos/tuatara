#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/named_wildcard.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, NamedWildcardJSON) {
    stringstream  ss;
    yy::location  loc;
    NamedWildcard node(loc, "foo");
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"named wildcard",)"
        R"("name":"foo"})");

    ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << endl; });
    EXPECT_EQ(walk.str(), "Named Wildcard Pattern\n");
}

TEST(PatternTest, NamedWildcardWalk) {
    stringstream  ss;
    yy::location  loc;
    NamedWildcard node(loc, "foo");
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Named Wildcard Pattern\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif