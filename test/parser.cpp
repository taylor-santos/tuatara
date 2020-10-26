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

TEST(ParserTest, NoneLiteral) {
    std::istringstream iss("none;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::None &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(ss.str(), R"({"node":"none"})");
    }) << "Expected AST node to be a None";
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

TEST(ParserTest, TypedVariable) {
    std::istringstream iss("abc: int;");
    std::ostringstream oss;
    yy::Driver         drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement";
    EXPECT_NO_THROW({
        const auto &       node = dynamic_cast<AST::TypedVariable &>(*drv.statements[0]);
        std::ostringstream ss;
        ss << node;
        EXPECT_EQ(
            ss.str(),
            R"({"node":"typed variable",)"
            R"("name":"abc",)"
            R"("type":{)"
            R"("type":"object",)"
            R"("class":"int"}})");
    }) << "Expected AST node to be a TypedVariable";
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
            R"("type":"object",)"
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
            R"("type":"object",)"
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
