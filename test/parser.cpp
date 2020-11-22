#include "gtest/gtest.h"
#include "driver.h"
#include "common.h"

TEST(ParserTest, EmptyFile) {
    std::istringstream iss("");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    EXPECT_TRUE(drv.statements.empty()) << "Expected statements list to be empty";
}

TEST(ParserTest, IntLiteral) {
    std::istringstream iss("123;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Int &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"int","value":123})");
    }) << "Expected AST node to be an Int";
}

TEST(ParserTest, FloatLiteral) {
    std::istringstream iss("123.456;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Float &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"float","value":123.456})");
    }) << "Expected AST node to be a Float";
}

TEST(ParserTest, StringLiteral) {
    std::istringstream iss(R"("foobar";)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::String &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"string","value":"foobar"})");
    }) << "Expected AST node to be a String";
}

TEST(ParserTest, BoolLiteral) {
    std::istringstream iss("true; false;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 2) << "Expected statements list to have two statements";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Bool &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"bool","value":true})");
    }) << "Expected AST node to be a Bool";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Bool &>(*drv.statements[1]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"bool","value":false})");
    }) << "Expected AST node to be a Bool";
}

TEST(ParserTest, Variable) {
    std::istringstream iss("abc;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Variable &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"variable","name":"abc"})");
    }) << "Expected AST node to be a Variable";
}

TEST(ParserTest, Assignment) {
    std::istringstream iss("abc = 5;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Assignment &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"assignment",)"
            R"("lhs":{)"
            R"("node":"variable",)"
            R"("name":"abc"},)"
            R"("rhs":{)"
            R"("node":"int",)"
            R"("value":5}})");
    }) << "Expected AST node to be an Assignment";
}

TEST(ParserTest, ValueDeclaration) {
    std::istringstream iss("var abc = 123;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::ValueDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"value declaration",)"
            R"("variable":"abc",)"
            R"("value":{)"
            R"("node":"int",)"
            R"("value":123}})");
    }) << "Expected AST node to be a ValueDeclaration";
}

TEST(ParserTest, TypeDeclaration) {
    std::istringstream iss("var abc: int;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"abc",)"
            R"("type":{)"
            R"("kind":"object",)"
            R"("class":"int"}})");
    }) << "Expected AST node to be a TypeDeclaration";
}

TEST(ParserTest, TypeValueDeclaration) {
    std::istringstream iss("var abc: int = 123;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeValueDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type value declaration",)"
            R"("variable":"abc",)"
            R"("type":{)"
            R"("kind":"object",)"
            R"("class":"int"},)"
            R"("value":{)"
            R"("node":"int",)"
            R"("value":123}})");
    }) << "Expected AST node to be a TypeValueDeclaration";
}

TEST(ParserTest, Block) {
    std::istringstream iss("{\n\t5;\n\t12.34;\n\tfalse;\n}");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Block &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"block",)"
            R"("statements":[{)"
            R"("node":"int",)"
            R"("value":5},)"
            R"({"node":"float",)"
            R"("value":12.34},)"
            R"({"node":"bool",)"
            R"("value":false}]})");
    }) << "Expected AST node to be a Block";
}

TEST(ParserTest, IfStatement) {
    std::istringstream iss(R"(if true 123;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::If &>(*drv.statements[0]);
        std::ostringstream ss;
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
    }) << "Expected AST node to be an If Statement";
}

TEST(ParserTest, IfStatementElseStatement) {
    std::istringstream iss(R"(if true 123; else 456;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::If &>(*drv.statements[0]);
        std::ostringstream ss;
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
            R"("node":"int",)"
            R"("value":456}})");
    }) << "Expected AST node to be an If Statement";
}

TEST(ParserTest, WhileStatement) {
    std::istringstream iss(R"(while true 123;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::While &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"while",)"
            R"("cond":{)"
            R"("node":"bool",)"
            R"("value":true},)"
            R"("statement":{)"
            R"("node":"int",)"
            R"("value":123}})");
    }) << "Expected AST node to be a While Statement";
}

TEST(ParserTest, ReturnStatement) {
    std::istringstream iss(R"(return;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Return &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"return"})");
    }) << "Expected AST node to be a Return Statement";
}

TEST(ParserTest, ReturnValueStatement) {
    std::istringstream iss(R"(return 123;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Return &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"return",)"
            R"("returns":{)"
            R"("node":"int",)"
            R"("value":123}})");
    }) << "Expected AST node to be a Return Statement";
}

TEST(ParserTest, FunctionCallNoArgs) {
    std::istringstream iss(R"(foo();)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Call &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"function call",)"
            R"("function":{)"
            R"("node":"variable",)"
            R"("name":"foo"},)"
            R"("args":[]})");
    }) << "Expected AST node to be a Function Call";
}

TEST(ParserTest, FunctionCallOneArg) {
    std::istringstream iss(R"(foo(123);)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Call &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"function call",)"
            R"("function":{)"
            R"("node":"variable",)"
            R"("name":"foo"},)"
            R"("args":[{)"
            R"("node":"int",)"
            R"("value":123}]})");
    }) << "Expected AST node to be a Function Call";
}

TEST(ParserTest, FunctionCallTwoArgs) {
    std::istringstream iss(R"(foo(123, 456);)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::Call &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"function call",)"
            R"("function":{)"
            R"("node":"variable",)"
            R"("name":"foo"},)"
            R"("args":[{)"
            R"("node":"int",)"
            R"("value":123},)"
            R"({"node":"int",)"
            R"("value":456}]})");
    }) << "Expected AST node to be a Function Call";
}

TEST(ParserTest, NullaryVoidFuncTypeDeclaration) {
    std::istringstream iss(R"(var fn: ->;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"fn",)"
            R"("type":{)"
            R"("kind":"func"}})");
    }) << "Expected AST node to be a Function Type Declaration";
}

TEST(ParserTest, VoidFuncTypeDeclaration) {
    std::istringstream iss(R"(var fn: int->;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"fn",)"
            R"("type":{)"
            R"("kind":"func",)"
            R"("arg":{)"
            R"("kind":"object",)"
            R"("class":"int"}}})");
    }) << "Expected AST node to be a Function Type Declaration";
}

TEST(ParserTest, NullaryFuncTypeDeclaration) {
    std::istringstream iss(R"(var fn: ->int;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"fn",)"
            R"("type":{)"
            R"("kind":"func",)"
            R"("returns":{)"
            R"("kind":"object",)"
            R"("class":"int"}}})");
    }) << "Expected AST node to be a Function Type Declaration";
}

TEST(ParserTest, FuncTypeDeclaration) {
    std::istringstream iss(R"(var fn: int->int;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"fn",)"
            R"("type":{)"
            R"("kind":"func",)"
            R"("arg":{)"
            R"("kind":"object",)"
            R"("class":"int"},)"
            R"("returns":{)"
            R"("kind":"object",)"
            R"("class":"int"}}})");
    }) << "Expected AST node to be a Function Type Declaration";
}

TEST(ParserTest, FuncTypeDeclarationPrecedence) {
    /* The type expression a -> b -> c should be equivalent to a -> (b -> c) */
    std::istringstream iss(R"(var fn: a -> b -> c;)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"fn",)"
            R"("type":{)"
            R"("kind":"func",)"
            R"("arg":{)"
            R"("kind":"object",)"
            R"("class":"a"},)"
            R"("returns":{)"
            R"("kind":"func",)"
            R"("arg":{)"
            R"("kind":"object",)"
            R"("class":"b"},)"
            R"("returns":{)"
            R"("kind":"object",)"
            R"("class":"c"}}}})");
    }) << "Expected AST node to be a Function Type Declaration";
}

TEST(ParserTest, MultidimensionalArrays) {
    std::istringstream iss(R"(var arr: int[][];)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"arr",)"
            R"("type":{)"
            R"("kind":"array",)"
            R"("type":{)"
            R"("kind":"array",)"
            R"("type":{)"
            R"("kind":"object",)"
            R"("class":"int"}}}})");
    }) << "Expected AST node to be an Array Type Declaration";
}

TEST(ParserTest, TupleTypeDeclaration) {
    std::istringstream iss(R"(var tup: (A, B);)");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypeDeclaration &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"type declaration",)"
            R"("variable":"tup",)"
            R"("type":{)"
            R"("kind":"tuple",)"
            R"("types":[{)"
            R"("kind":"object",)"
            R"("class":"A"},)"
            R"({"kind":"object",)"
            R"("class":"B"}]}})");
    }) << "Expected AST node to be a Tuple Type Declaration";
}

TEST(ParserTest, MissingSemicolon) {
    std::istringstream iss("123");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 1) << "Expected Bison to return an error code";
    EXPECT_EQ(
        oss.str(),
        "1:4: syntax error, unexpected end of file, expecting ;\n"
        "1 | 123\n"
        "  |    ^\n");
}
