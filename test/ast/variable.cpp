#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/variable.h"

#include "type/object.h"
#include "type/type_exception.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, VariableJSON) {
    ostringstream ss;
    yy::location  loc;
    Variable      node(loc, "var");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"variable","name":"var"})");
}

TEST(ASTTest, VariableWalk) {
    ostringstream ss;
    yy::location  loc;
    Variable      node(loc, "var");
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Variable\n");
}

TEST(ASTTest, VariableGetType) {
    istringstream iss("var b = 123\n"
                      "var a = b");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, UndefinedVariableGetType) {
    istringstream iss("var a = b");

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
            ASSERT_EQ(e.getMsgs().size(), 1);
            EXPECT_EQ(e.getMsgs().at(0).first, "use of unidentified variable \"b\"");
            throw;
        },
        TypeChecker::TypeException);
}

TEST(ASTTest, UninitializedVariableGetType) {
    istringstream iss("var b: int\n"
                      "var a = b");

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
            ASSERT_EQ(e.getMsgs().size(), 2);
            EXPECT_EQ(e.getMsgs().at(0).first, "use of uninitialized variable \"b\"");
            EXPECT_EQ(
                e.getMsgs().at(1).first,
                "variable \"b\" was not initialized when it was declared");
            throw;
        },
        TypeChecker::TypeException);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
