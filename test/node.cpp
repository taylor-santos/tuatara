#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include <memory>

#include "common.h"
#include "gtest/gtest.h"

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

TEST(ASTTest, InfixOperatorNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               lhs = make_unique<Variable>(loc, "var");
    auto               rhs = make_unique<Int>(loc, 123);
    InfixOperator      node(loc, "+", move(lhs), move(rhs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"infix operator",)"
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
    auto               type = make_unique<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration    node(loc, "var", move(type));
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
    auto                 type = make_unique<TypeChecker::Object>(loc, "class_name");
    auto                 val  = make_unique<Int>(loc, 123);
    TypeValueDeclaration node(loc, "var", move(type), move(val));
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
    Expression::Vec    stmts;
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
    std::ostringstream           ss;
    yy::location                 loc;
    auto                         cond = make_unique<Bool>(loc, true);
    vector<AST::Expression::Ptr> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto block = make_unique<Block>(loc, move(stmts));
    If   node(loc, move(cond), move(block));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]}})");
}

TEST(ASTTest, IfElseNodeJSON) {
    std::ostringstream           ss;
    yy::location                 loc;
    auto                         cond = make_unique<Bool>(loc, true);
    vector<AST::Expression::Ptr> trueStmts;
    trueStmts.emplace_back(make_unique<Int>(loc, 123));
    auto                         trueBlock = make_unique<Block>(loc, move(trueStmts));
    vector<AST::Expression::Ptr> falseStmts;
    falseStmts.emplace_back(make_unique<Float>(loc, 4.56));
    auto   falseBlock = make_unique<Block>(loc, move(falseStmts));
    IfElse node(loc, move(cond), move(trueBlock), move(falseBlock));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if else",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]},)"
        R"("if false":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"float",)"
        R"("value":4.56}]}})");
}

TEST(ASTTest, WhileNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               cond = make_unique<Bool>(loc, true);
    Expression::Vec    stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto  block = make_unique<AST::Block>(loc, move(stmts));
    While node(loc, move(cond), move(block));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"while\","
        "\"cond\":{"
        "\"node\":\"bool\","
        "\"value\":true},"
        "\"block\":{"
        R"("node":"block",)"
        R"("statements":[{)"
        "\"node\":\"int\","
        "\"value\":123}]}}");
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

TEST(ASTTest, IndexNodeJSON) {
    std::ostringstream ss;
    yy::location       loc;
    auto               expr  = make_unique<Variable>(loc, "a");
    auto               index = make_unique<Variable>(loc, "b");
    Index              node(loc, move(expr), move(index));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"array index",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("index":{)"
        R"("node":"variable",)"
        R"("name":"b"}})");
}

TEST(ASTTest, FuncDeclarationNodeJSON) {
    std::ostringstream    ss;
    yy::location          loc;
    string                name = "foo";
    Pattern::Pattern::Vec args;
    auto                  argType        = make_unique<TypeChecker::Object>(loc, "S");
    auto                  typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto            ret = make_unique<TypeChecker::Object>(loc, "T");
    FuncDeclaration node(loc, name, move(args), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function declaration",)"
        R"("variable":"foo",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"arg",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"S"}}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");
}

TEST(ASTTest, FuncImplNodeJSON) {
    std::ostringstream    ss;
    yy::location          loc;
    string                name = "foo";
    Pattern::Pattern::Vec args;
    auto                  argType        = make_unique<TypeChecker::Object>(loc, "S");
    auto                  typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto            ret = make_unique<TypeChecker::Object>(loc, "T");
    Expression::Vec stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto     block = make_unique<Block>(loc, move(stmts));
    FuncImpl node(loc, name, move(args), move(block), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function impl",)"
        R"("variable":"foo",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"arg",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"S"}}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"},)"
        R"("body":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"b"}]}})");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
