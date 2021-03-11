#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/value_constraint.h"

#include "ast/int.h"

#include "type/object.h"
#include "type/type_context.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, ValueConstraintJSON) {
    stringstream    ss;
    yy::location    loc;
    auto            value = make_unique<AST::Int>(loc, 123);
    ValueConstraint node(loc, move(value));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");

    ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << endl; });
    EXPECT_EQ(walk.str(), "Value Constraint Pattern\nInt\n");
}

TEST(PatternTest, ValueConstraintWalk) {
    stringstream    ss;
    yy::location    loc;
    auto            value = make_unique<AST::Int>(loc, 123);
    ValueConstraint node(loc, move(value));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Value Constraint Pattern\nInt\n");
}

TEST(PatternTest, ValueConstraintGetType) {
    yy::location         loc;
    auto                 value = make_unique<AST::Int>(loc, 123);
    ValueConstraint      node(loc, move(value));
    TypeChecker::Context ctx;

    shared_ptr<TypeChecker::Type> target = make_shared<TypeChecker::Object>(loc, "int");
    target                               = target->verify(ctx);
    auto type                            = node.getType(ctx);
    EXPECT_TRUE(type->isEqual(*target, ctx));
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
