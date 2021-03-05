#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/unit.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, UnitJSON) {
    stringstream ss;
    yy::location loc;
    Unit         node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"unit"})");
}

TEST(TypeTest, UnitWalk) {
    stringstream ss;
    yy::location loc;
    Unit         node(loc);
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Unit Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
