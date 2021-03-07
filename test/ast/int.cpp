#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/int.h"

#include "test_util.h"

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

TEST(ASTTest, IntGetType) {
    istringstream iss("var a = 123");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
