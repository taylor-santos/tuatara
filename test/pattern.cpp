#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include <memory>

#include "ast/int.h"

#include "pattern/literal.h"
#include "pattern/named_constraint.h"
#include "pattern/named_wildcard.h"
#include "pattern/tuple.h"
#include "pattern/type_constraint.h"
#include "pattern/value_constraint.h"
#include "pattern/wildcard.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace std;

TEST(PatternTest, WildcardNode) {
    yy::location      loc;
    Pattern::Wildcard node(loc);
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(ss.str(), R"({"pattern":"wildcard"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Wildcard Pattern\n");
}

TEST(PatternTest, NamedWildcardNode) {
    yy::location           loc;
    Pattern::NamedWildcard node(loc, "foo");
    std::stringstream      ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"named wildcard",)"
        R"("name":"foo"})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Named Wildcard Pattern\n");
}

TEST(PatternTest, TypeConstraintNode) {
    yy::location            loc;
    auto                    type = make_unique<TypeChecker::Object>(loc, "T");
    Pattern::TypeConstraint node(loc, move(type));
    std::stringstream       ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Type Constraint Pattern\nObject Type\n");
}

TEST(PatternTest, ValueConstraintNode) {
    yy::location             loc;
    auto                     value = make_unique<AST::Int>(loc, 123);
    Pattern::ValueConstraint node(loc, move(value));
    std::stringstream        ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Value Constraint Pattern\nInt\n");
}

TEST(PatternTest, LiteralNode) {
    yy::location      loc;
    auto              value = make_unique<AST::Int>(loc, 123);
    Pattern::Literal  node(loc, move(value));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"literal",)"
        R"("literal":{)"
        R"("node":"int",)"
        R"("value":123}})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Literal Pattern\nInt\n");
}

TEST(PatternTest, TupleNode) {
    yy::location                         loc;
    auto                                 value = make_unique<AST::Int>(loc, 123);
    vector<unique_ptr<Pattern::Pattern>> patterns;
    patterns.emplace_back(make_unique<Pattern::ValueConstraint>(loc, move(value)));
    patterns.emplace_back(make_unique<Pattern::Wildcard>(loc));
    Pattern::Tuple    node(loc, move(patterns));
    std::stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"pattern":"tuple",)"
        R"("patterns":[{)"
        R"("pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}},{)"
        R"("pattern":"wildcard"}]})");

    std::ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << std::endl; });
    EXPECT_EQ(walk.str(), "Tuple Pattern\nValue Constraint Pattern\nInt\nWildcard Pattern\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
