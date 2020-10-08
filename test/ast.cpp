#include "gtest/gtest.h"
#include "location.hh"
#include "json.h"

#include "ast/int.h"
#include "ast/float.h"
#include "ast/string.h"
#include "ast/variable.h"
#include "ast/assignment.h"
#include "ast/value_declaration.h"
#include "ast/type_declaration.h"

#include "type/object.h"

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
    auto               val = make_unique<Int>(loc, 123);
    Assignment         node(loc, "var", move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        "{\"node\":\"assignment\","
        "\"variable\":\"var\","
        "\"value\":{"
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
