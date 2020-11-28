#include "gtest/gtest.h"
#include "location.hh"
#include "common.h"

#include <memory>

using namespace AST;
using namespace std;

TEST(ASTTest, IntNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Int                node(loc, 123);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"int","value":123})");
}

TEST(ASTTest, FloatNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Float              node(loc, 123.456);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"float","value":123.456})");
}

TEST(ASTTest, StringNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    String             node(loc, "StringNode");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"string","value":"StringNode"})");
}

TEST(ASTTest, TrueBoolNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Bool               node(loc, true);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"bool","value":true})");
}

TEST(ASTTest, FalseBoolNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Bool               node(loc, false);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"bool","value":false})");
}

TEST(ASTTest, VariableNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Variable           node(loc, "var");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"variable","name":"var"})");
}

TEST(ASTTest, OperatorNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               lhs = make_unique<Variable>(loc, "var");
    auto               rhs = make_unique<Int>(loc, 123);
    Operator           node(loc, "+", move(lhs), move(rhs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"operator",)"
        R"("operation":"+",)"
        R"("lhs":{)"
        R"("node":"variable",)"
        R"("name":"var"},)"
        R"("rhs":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, ValueDeclarationNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               val = make_unique<Int>(loc, 123);
    ValueDeclaration   node(loc, "var", move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"value declaration",)"
        R"("variable":"var",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, TypeDeclarationNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               type = make_shared<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration    node(loc, "var", type);
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"type declaration",)"
        R"("variable":"var",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"class_name"}})");
}

TEST(ASTTest, TypeValueDeclarationNodeJSON) {
    std::ostringstream   ss;
    yy::location         loc;
    auto                 type = make_shared<TypeChecker::Object>(loc, "class_name");
    auto                 val  = make_unique<Int>(loc, 123);
    TypeValueDeclaration node(loc, "var", type, move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"type value declaration",)"
        R"("variable":"var",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"class_name"},)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, BlockNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Statement::Vec     stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    stmts.emplace_back(make_unique<Float>(loc, 45.6));
    Block node(loc, move(stmts));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123},)"
        R"({"node":"float",)"
        R"("value":45.6}]})");
}

TEST(ASTTest, IfNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               cond = make_unique<Bool>(loc, true);
    auto               stmt = make_unique<Int>(loc, 123);
    If                 node(loc, move(cond), move(stmt));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("statement":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, IfElseNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               cond      = make_unique<Bool>(loc, true);
    auto               stmt      = make_unique<Int>(loc, 123);
    auto               else_stmt = make_unique<Float>(loc, 4.56);
    If                 node(loc, move(cond), move(stmt), move(else_stmt));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("statement":{)"
        R"("node":"int",)"
        R"("value":123},)"
        R"("else":{)"
        R"("node":"float",)"
        R"("value":4.56}})");
}

TEST(ASTTest, WhileNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               cond = make_unique<Bool>(loc, true);
    auto               stmt = make_unique<Int>(loc, 123);
    While              node(loc, move(cond), move(stmt));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"while\","
        "\"cond\":{"
        "\"node\":\"bool\","
        "\"value\":true},"
        "\"statement\":{"
        "\"node\":\"int\","
        "\"value\":123}}");
}

TEST(ASTTest, CallNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               func = make_unique<Variable>(loc, "a");
    auto               arg  = make_unique<Variable>(loc, "b");
    Call               node(loc, move(func), move(arg));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("arg":{)"
        R"("node":"variable",)"
        R"("name":"b"}})");
}

TEST(ASTTest, FuncDeclarationNodeJSON) {
    std::ostringstream                           ss;
    yy::location                                 loc;
    string                                       name = "foo";
    vector<pair<string, TypeChecker::Type::Ptr>> args;
    args.emplace_back(make_pair("arg", make_shared<TypeChecker::Object>(loc, "S")));
    auto            ret  = make_shared<TypeChecker::Object>(loc, "T");
    auto            stmt = make_unique<Variable>(loc, "b");
    FuncDeclaration node(loc, name, move(args), move(ret), move(stmt));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function declaration",)"
        R"("variable":"foo",)"
        R"("args":[{)"
        R"("name":"arg",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"S"}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"},)"
        R"("body":{)"
        R"("node":"variable",)"
        R"("name":"b"}})");
}

TEST(ASTTest, ReturnNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    Return             node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"return"})");
}

TEST(ASTTest, ReturnValueNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               val = make_unique<Int>(loc, 5);
    Return             node(loc, move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"return",)"
        R"("returns":{)"
        R"("node":"int",)"
        R"("value":5}})");
}