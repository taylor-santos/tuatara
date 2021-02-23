#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include <memory>

#include "type/array.h"
#include "type/func.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/sum.h"
#include "type/unit.h"

#include "gtest/gtest.h"

using namespace std;

TEST(TypeTest, ObjectNode) {
    yy::location        loc;
    TypeChecker::Object node(loc, "class_name");
    std::stringstream   ss;
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"object","class":"class_name"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Object Type\n");
}

TEST(TypeTest, ArrayNode) {
    yy::location       loc;
    auto               obj = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Array node(loc, move(obj));
    std::stringstream  ss;
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"array","type":{"kind":"object","class":"T"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Array Type\nObject Type\n");
}

TEST(TypeTest, ProductNode) {
    yy::location           loc;
    TypeChecker::Type::Vec types;
    types.reserve(2);
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "S"));
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "T"));
    TypeChecker::Product node(loc, move(types));
    std::stringstream    ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"product",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Product Type\nObject Type\nObject Type\n");
}

TEST(TypeTest, SumNode) {
    yy::location           loc;
    TypeChecker::Type::Vec types;
    types.reserve(2);
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "S"));
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "T"));
    TypeChecker::Sum  node(loc, move(types));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"sum",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Sum Type\nObject Type\nObject Type\n");
}

TEST(TypeTest, MaybeNode) {
    yy::location       loc;
    auto               obj = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Maybe node(loc, move(obj));
    std::stringstream  ss;
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"maybe","type":{"kind":"object","class":"T"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Maybe Type\nObject Type\n");
}

TEST(TypeTest, NullaryVoidFuncNode) {
    yy::location      loc;
    auto              arg = make_unique<TypeChecker::Unit>(loc);
    auto              ret = make_unique<TypeChecker::Unit>(loc);
    TypeChecker::Func node(loc, move(arg), move(ret));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func",)"
        R"("arg":{)"
        R"("kind":"unit"},)"
        R"("return type":{)"
        R"("kind":"unit"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Func Type\nUnit Type\nUnit Type\n");
}

TEST(TypeTest, NullaryFuncNode) {
    yy::location      loc;
    auto              arg = make_unique<TypeChecker::Unit>(loc);
    auto              ret = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func node(loc, move(arg), move(ret));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func",)"
        R"("arg":{)"
        R"("kind":"unit"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Func Type\nUnit Type\nObject Type\n");
}

TEST(TypeTest, VoidFuncNode) {
    yy::location      loc;
    auto              ret = make_unique<TypeChecker::Unit>(loc);
    auto              arg = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func node(loc, move(arg), move(ret));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"T"},)"
        R"("return type":{)"
        R"("kind":"unit"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Func Type\nObject Type\nUnit Type\n");
}

TEST(TypeTest, FuncNode) {
    yy::location      loc;
    auto              arg = make_unique<TypeChecker::Object>(loc, "S");
    auto              ret = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func node(loc, move(arg), move(ret));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"S"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Func Type\nObject Type\nObject Type\n");
}

TEST(TypeTest, UnitNode) {
    yy::location      loc;
    TypeChecker::Unit node(loc);
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(ss.str(), R"({"kind":"unit"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Unit Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
