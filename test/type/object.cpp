#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, ObjectJSON) {
    stringstream ss;
    yy::location      loc;
    Object            node(loc, "class_name");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"object","class":"class_name"})");
}

TEST(TypeTest, ObjectWalk) {
    stringstream ss;
    yy::location      loc;
    Object            node(loc, "class_name");
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Object Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
