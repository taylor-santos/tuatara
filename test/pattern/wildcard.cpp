#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/wildcard.h"

#include "type/type_context.h"
#include "type/unit.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, WildcardJSON) {
    stringstream ss;
    yy::location loc;
    Wildcard     node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"pattern":"wildcard"})");
}

TEST(PatternTest, WildcardWalk) {
    stringstream ss;
    yy::location loc;
    Wildcard     node(loc);
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Wildcard Pattern\n");
}

TEST(PatternTest, WildcardGetType) {
    yy::location                  loc;
    Wildcard                      node(loc);
    TypeChecker::Context          ctx;
    shared_ptr<TypeChecker::Type> target = make_shared<TypeChecker::Unit>(loc);
    target                               = target->verify(ctx);
    auto type                            = node.getType(ctx);
    EXPECT_TRUE(type->isEqual(*target, ctx));
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
