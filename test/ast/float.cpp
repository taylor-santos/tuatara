#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/float.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, FloatJSON) {
    ostringstream ss;
    yy::location  loc;
    Float         node(loc, 123.456);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"float","value":123.456})");
}

TEST(ASTTest, FloatWalk) {
    ostringstream ss;
    yy::location  loc;
    Float         node(loc, 123.456);
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Float\n");
}

TEST(ASTTest, FloatGetType) {
    istringstream iss("var a = 12.34");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "float");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
