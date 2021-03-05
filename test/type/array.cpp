#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/array.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, ArrayJSON) {
    ostringstream ss;
    yy::location  loc;
    auto          obj = make_unique<Object>(loc, "T");
    Array         node(loc, move(obj));
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"array","type":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, ArrayWalk) {
    ostringstream ss;
    yy::location  loc;
    auto          obj = make_unique<Object>(loc, "T");
    Array         node(loc, move(obj));

    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Array Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
