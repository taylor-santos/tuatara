#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/maybe.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, MaybeJSON) {
    stringstream ss;
    yy::location loc;
    auto         obj = make_unique<Object>(loc, "T");
    Maybe        node(loc, move(obj));
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"maybe","type":{"kind":"object","class":"T"}})");

    ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << endl; });
    EXPECT_EQ(walk.str(), "Maybe Type\nObject Type\n");
}

TEST(TypeTest, MaybeWalk) {
    stringstream ss;
    yy::location loc;
    auto         obj = make_unique<Object>(loc, "T");
    Maybe        node(loc, move(obj));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Maybe Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
