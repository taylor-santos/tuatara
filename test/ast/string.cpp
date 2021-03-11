#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/string.h"

#include "type/object.h"

#include "test_util.h"

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

TEST(ASTTest, StringGetType) {
    istringstream iss(R"(var a = "foo")");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "string");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
