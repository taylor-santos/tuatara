#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include <memory>

#include "common.h"
#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IntNode) {
    std::ostringstream ss;
    yy::location       loc;
    Int                node(loc, 123);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"int","value":123})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Int\n");
}

TEST(ASTTest, FloatNode) {
    std::ostringstream ss;
    yy::location       loc;
    Float              node(loc, 123.456);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"float","value":123.456})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Float\n");
}

TEST(ASTTest, StringNode) {
    std::ostringstream ss;
    yy::location       loc;
    String             node(loc, "StringNode");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"string","value":"StringNode"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "String\n");
}

TEST(ASTTest, TrueBoolNode) {
    std::ostringstream ss;
    yy::location       loc;
    Bool               node(loc, true);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"bool","value":true})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Bool\n");
}

TEST(ASTTest, FalseBoolNode) {
    std::ostringstream ss;
    yy::location       loc;
    Bool               node(loc, false);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"bool","value":false})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Bool\n");
}

TEST(ASTTest, VariableNode) {
    std::ostringstream ss;
    yy::location       loc;
    Variable           node(loc, "var");
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"variable","name":"var"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Variable\n");
}

TEST(ASTTest, ValueDeclarationNode) {
    std::ostringstream ss;
    yy::location       loc;
    auto               val = make_unique<Int>(loc, 123);
    ValueDeclaration   node(loc, loc, "var", move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"value declaration",)"
        R"("variable":"var",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Value Decl\nInt\n");
}

TEST(ASTTest, TypeDeclarationNode) {
    std::ostringstream ss;
    yy::location       loc;
    auto               type = make_unique<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration    node(loc, loc, "var", move(type));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"type declaration",)"
        R"("variable":"var",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"class_name"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Type Decl\nObject Type\n");
}

TEST(ASTTest, TypeValueDeclarationNode) {
    std::ostringstream   ss;
    yy::location         loc;
    auto                 type = make_unique<TypeChecker::Object>(loc, "class_name");
    auto                 val  = make_unique<Int>(loc, 123);
    TypeValueDeclaration node(loc, loc, "var", move(type), move(val));
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Type Value Decl\nObject Type\nInt\n");
}

TEST(ASTTest, BlockNode) {
    std::ostringstream             ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> stmts;
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Block\nInt\nFloat\n");
}

TEST(ASTTest, IfNode) {
    std::ostringstream                  ss;
    yy::location                        loc;
    auto                                cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<AST::Expression>> stmts;
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "If\nBool\nBlock\nInt\n");
}

TEST(ASTTest, IfElseNode) {
    std::ostringstream                  ss;
    yy::location                        loc;
    auto                                cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<AST::Expression>> trueStmts;
    trueStmts.emplace_back(make_unique<Int>(loc, 123));
    auto                                trueBlock = make_unique<Block>(loc, move(trueStmts));
    vector<unique_ptr<AST::Expression>> falseStmts;
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "If Else\nBool\nBlock\nInt\nBlock\nFloat\n");
}

TEST(ASTTest, WhileNode) {
    std::ostringstream             ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> stmts;
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "While\nBool\nBlock\nInt\n");
}

TEST(ASTTest, CallNode) {
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Call\nVariable\nVariable\n");
}

TEST(ASTTest, IndexNode) {
    std::ostringstream ss;
    yy::location       loc;
    auto               expr  = make_unique<Variable>(loc, "a");
    auto               index = make_unique<Int>(loc, 5);
    Index              node(loc, move(expr), move(index));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"array index",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("index":{)"
        R"("node":"int",)"
        R"("value":5}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Index\nVariable\nInt\n");
}

TEST(ASTTest, FuncDeclarationNode) {
    std::ostringstream                   ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto            ret = make_unique<TypeChecker::Object>(loc, "T");
    FuncDeclaration node(loc, loc, name, move(args), move(ret));
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(
        walk.str(),
        "Func Decl\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\nObject Type\n");
}

TEST(ASTTest, FuncImplNode) {
    std::ostringstream                   ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_unique<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto     block = make_unique<Block>(loc, move(stmts));
    FuncImpl node(loc, loc, name, move(args), move(block), move(ret));
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(
        walk.str(),
        "Func Impl\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\nObject Type\n"
        "Block\nVariable\n");
}

TEST(ASTTest, LambdaNode) {
    std::ostringstream                   ss;
    yy::location                         loc;
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_unique<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto   block = make_unique<Block>(loc, move(stmts));
    Lambda node(loc, move(args), move(ret), move(block));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"lambda",)"
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

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(
        walk.str(),
        "Lambda\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\n"
        "Object Type\nBlock\nVariable\n");
}

TEST(ASTTest, TupleNode) {
    std::ostringstream             ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> exprs;
    exprs.emplace_back(make_unique<Variable>(loc, "var"));
    exprs.emplace_back(make_unique<Int>(loc, 123));
    Tuple node(loc, move(exprs));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"tuple",)"
        R"("expressions":[{)"
        R"("node":"variable",)"
        R"("name":"var"},{)"
        R"("node":"int",)"
        R"("value":123}]})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Tuple\nVariable\nInt\n");
}

TEST(ASTTest, UnitNode) {
    std::ostringstream ss;
    yy::location       loc;
    Unit               node(loc);
    ss << node;
    EXPECT_EQ(ss.str(), R"({"node":"unit"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Unit\n");
}

TEST(ASTTest, MatchNode) {
    std::ostringstream       ss;
    yy::location             loc;
    std::vector<Match::Case> cases;
    auto                     val  = make_unique<AST::Int>(loc, 5);
    auto                     pat  = make_unique<Pattern::Literal>(loc, move(val));
    auto                     expr = make_unique<AST::Variable>(loc, "bar");
    cases.emplace_back(make_pair(move(pat), move(expr)));
    AST::Match node(loc, make_unique<AST::Variable>(loc, "foo"), move(cases));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"match",)"
        R"("value":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("cases":[{)"
        R"("pattern":{)"
        R"("pattern":"literal",)"
        R"("literal":{)"
        R"("node":"int",)"
        R"("value":5}},)"
        R"("body":{)"
        R"("node":"variable",)"
        R"("name":"bar"}}]})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Match\nVariable\nLiteral Pattern\nInt\nVariable\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
