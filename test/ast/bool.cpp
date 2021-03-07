#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/bool.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, BoolJSON) {
    ostringstream ss;
    yy::location  loc;
    Bool          node(loc, true);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"bool","value":true})");
}

TEST(ASTTest, BoolWalk) {
    ostringstream ss;
    yy::location  loc;
    Bool          node(loc, true);
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Bool\n");
}

TEST(ASTTest, BoolGetType) {
    istringstream iss("var a = true");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "bool");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
