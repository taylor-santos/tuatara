#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/array.h"

#include "ast/int.h"
#include "ast/variable.h"

#include "type/array.h"
#include "type/object.h"
#include "type/type_exception.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, ArrayJSON) {
    ostringstream                  ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> exprs;
    exprs.emplace_back(make_unique<Int>(loc, 123));
    exprs.emplace_back(make_unique<Int>(loc, 456));
    Array node(loc, move(exprs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"array",)"
        R"("expressions":[{)"
        R"("node":"int",)"
        R"("value":123},{)"
        R"("node":"int",)"
        R"("value":456}]})");
}

TEST(ASTTest, ArrayWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> exprs;
    exprs.emplace_back(make_unique<Int>(loc, 123));
    exprs.emplace_back(make_unique<Int>(loc, 456));
    Array node(loc, move(exprs));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Array\nInt\nInt\n");
}

TEST(ASTTest, ArrayGetType) {
    istringstream iss(R"(var a = [1,2,3,4])");

    yy::location loc;
    auto         target = make_shared<TypeChecker::Array>(
        yy::location{},
        make_unique<TypeChecker::Object>(loc, "int"));
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, ArrayIncompatibleGetType) {
    istringstream iss(R"(var a = [1, "foo"])");

    SETUP_SCANNER(iss);
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    TypeChecker::Context ctx;
    EXPECT_THROW(
        try {
            for (auto &stmt : drv.statements) {
                stmt->getType(ctx);
            }
        } catch (const TypeChecker::TypeException &e) {
            ASSERT_EQ(e.getMsgs().size(), 4);
            EXPECT_EQ(e.getMsgs().at(0).first, "array must have uniform type");
            EXPECT_EQ(
                e.getMsgs().at(1).first,
                "expression at position 1 has type \"string\" which conflicts with the previous "
                "element type \"int\"");
            EXPECT_EQ(e.getMsgs().at(2).first, "expression given type \"string\" here:");
            EXPECT_EQ(e.getMsgs().at(3).first, "first element given type \"int\" here:");
            throw;
        },
        TypeChecker::TypeException);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
