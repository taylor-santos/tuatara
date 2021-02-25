#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/literal.h"

#include "ast/int.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, LiteralJSON) {
    stringstream ss;
    yy::location loc;
    auto         value = make_unique<AST::Int>(loc, 123);
    Literal      node(loc, move(value));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"literal",)"
        R"("literal":{)"
        R"("node":"int",)"
        R"("value":123}})");

    ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << endl; });
    EXPECT_EQ(walk.str(), "Literal Pattern\nInt\n");
}

TEST(PatternTest, LiteralWalk) {
    stringstream ss;
    yy::location loc;
    auto         value = make_unique<AST::Int>(loc, 123);
    Literal      node(loc, move(value));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Literal Pattern\nInt\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
