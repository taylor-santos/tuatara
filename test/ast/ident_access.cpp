#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/ident_access.h"

#include "ast/variable.h"

#include "type/func.h"
#include "type/object.h"
#include "type/type_exception.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IdentAccessJSON) {
    ostringstream ss;
    yy::location  loc;
    IdentAccess   node(loc, make_unique<Variable>(loc, "a"), loc, "b");
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("ident":"b"})");
}

TEST(ASTTest, IdentAccessWalk) {
    ostringstream ss;
    yy::location  loc;
    IdentAccess   node(loc, make_unique<Variable>(loc, "a"), loc, "b");
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Ident Access\nVariable\n");
}

TEST(ASTTest, IdentAccessFieldGetType) {
    istringstream iss("var someInt = 123\n"
                      "var a = someInt +");

    yy::location                  loc;
    shared_ptr<TypeChecker::Type> argType = make_shared<TypeChecker::Object>(loc, "int"),
                                  retType = make_shared<TypeChecker::Object>(loc, "int");
    auto target = make_shared<TypeChecker::Func>(loc, move(argType), move(retType));
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, IdentAccessCallGetType) {
    istringstream iss("var someFunc = (x:int)->x\n"
                      "var arg = 5\n"
                      "var a = someFunc arg");

    yy::location loc;
    auto         target = make_shared<TypeChecker::Object>(loc, "int");
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, IdentAccessCallUndeclared) {
    istringstream iss("var someFunc = (x:int)->x\n"
                      "var a = someFunc arg");

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
            ASSERT_EQ(e.getMsgs().size(), 6);
            EXPECT_EQ(
                e.getMsgs().at(0).first,
                "\"arg\" cannot be applied to an expression with type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(1).first, "\"arg\" cannot be a field because:");
            EXPECT_EQ(
                e.getMsgs().at(2).first,
                "cannot access member \"arg\" of non-class type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(3).first, "expression given type \"int->int\" here:");
            EXPECT_EQ(
                e.getMsgs().at(4).first,
                "\"arg\" cannot be the argument to a function because:");
            EXPECT_EQ(e.getMsgs().at(5).first, "\"arg\" is not the name of a variable in scope");
            throw;
        },
        TypeChecker::TypeException);
}

TEST(ASTTest, IdentAccessCallUninitialized) {
    istringstream iss("var someFunc = (x:int)->x\n"
                      "var arg: int\n"
                      "var a = someFunc arg");

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
            ASSERT_EQ(e.getMsgs().size(), 6);
            EXPECT_EQ(
                e.getMsgs().at(0).first,
                "\"arg\" cannot be applied to an expression with type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(1).first, "\"arg\" cannot be a field because:");
            EXPECT_EQ(
                e.getMsgs().at(2).first,
                "cannot access member \"arg\" of non-class type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(3).first, "expression given type \"int->int\" here:");
            EXPECT_EQ(
                e.getMsgs().at(4).first,
                "\"arg\" cannot be the argument to a function because:");
            EXPECT_EQ(e.getMsgs().at(5).first, "the variable \"arg\" has not been initialized");
            throw;
        },
        TypeChecker::TypeException);
}

TEST(ASTTest, IdentAccessCallInvalidArg) {
    istringstream iss("var someFunc = (x:int)->x\n"
                      "var arg = \"foo\"\n"
                      "var a = someFunc arg");

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
            ASSERT_EQ(e.getMsgs().size(), 8);
            EXPECT_EQ(
                e.getMsgs().at(0).first,
                "\"arg\" cannot be applied to an expression with type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(1).first, "\"arg\" cannot be a field because:");
            EXPECT_EQ(
                e.getMsgs().at(2).first,
                "cannot access member \"arg\" of non-class type \"int->int\"");
            EXPECT_EQ(e.getMsgs().at(3).first, "expression given type \"int->int\" here:");
            EXPECT_EQ(
                e.getMsgs().at(4).first,
                "\"arg\" cannot be the argument to a function because:");
            EXPECT_EQ(
                e.getMsgs().at(5).first,
                "function \"int->int\" expects an argument with type \"int\" but got \"string\"");
            EXPECT_EQ(e.getMsgs().at(6).first, "argument given type \"string\" here:");
            EXPECT_EQ(e.getMsgs().at(7).first, "function given type \"int->int\" here:");
            throw;
        },
        TypeChecker::TypeException);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
