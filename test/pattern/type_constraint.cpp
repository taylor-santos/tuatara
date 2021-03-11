#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/type_constraint.h"

#include "type/object.h"
#include "type/type_context.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, TypeConstraintJSON) {
    stringstream   ss;
    yy::location   loc;
    auto           type = make_shared<TypeChecker::Object>(loc, "T");
    TypeConstraint node(loc, move(type));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");
}

TEST(PatternTest, TypeConstraintWalk) {
    stringstream   ss;
    yy::location   loc;
    auto           type = make_shared<TypeChecker::Object>(loc, "T");
    TypeConstraint node(loc, move(type));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Type Constraint Pattern\nObject Type\n");
}

TEST(PatternTest, TypeConstraintGetType) {
    yy::location         loc;
    TypeConstraint       node(loc, make_shared<TypeChecker::Object>(loc, "int"));
    TypeChecker::Context ctx;

    shared_ptr<TypeChecker::Type> target = make_shared<TypeChecker::Object>(loc, "int");
    target                               = target->verify(ctx);
    auto type                            = node.getType(ctx);
    EXPECT_TRUE(type->isEqual(*target, ctx));
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
