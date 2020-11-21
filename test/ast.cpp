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

TEST(ASTTest, AssignmentNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               lhs = make_unique<Variable>(loc, "var");
    auto               rhs = make_unique<Int>(loc, 123);
    Assignment         node(loc, move(lhs), move(rhs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"assignment\","
        "\"lhs\":{"
        "\"node\":\"variable\","
        "\"name\":\"var\"},"
        "\"rhs\":{"
        "\"node\":\"int\","
        "\"value\":123}}");
}

TEST(ASTTest, ValueDeclarationNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               val = make_unique<Int>(loc, 123);
    ValueDeclaration   node(loc, "var", move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"value declaration\","
        "\"variable\":\"var\","
        "\"value\":{"
        "\"node\":\"int\","
        "\"value\":123}}");
}

TEST(ASTTest, TypeDeclarationNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               type = make_shared<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration    node(loc, "var", type);
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"type declaration\","
        "\"variable\":\"var\","
        "\"type\":{"
        "\"type\":\"object\","
        "\"class\":\"class_name\"}}");
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
        "{\"node\":\"type value declaration\","
        "\"variable\":\"var\","
        "\"type\":{"
        "\"type\":\"object\","
        "\"class\":\"class_name\"},"
        "\"value\":{"
        "\"node\":\"int\","
        "\"value\":123}}");
}

TEST(ASTTest, BlockNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    StatementVec       stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    stmts.emplace_back(make_unique<Float>(loc, 45.6));
    Block node(loc, move(stmts));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"block\","
        "\"statements\":[{"
        "\"node\":\"int\","
        "\"value\":123},"
        "{\"node\":\"float\","
        "\"value\":45.6}]}");
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
        "{\"node\":\"if\","
        "\"cond\":{"
        "\"node\":\"bool\","
        "\"value\":true},"
        "\"statement\":{"
        "\"node\":\"int\","
        "\"value\":123}}");
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
        "{\"node\":\"if\","
        "\"cond\":{"
        "\"node\":\"bool\","
        "\"value\":true},"
        "\"statement\":{"
        "\"node\":\"int\","
        "\"value\":123},"
        "\"else\":{"
        "\"node\":\"float\","
        "\"value\":4.56}}");
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
