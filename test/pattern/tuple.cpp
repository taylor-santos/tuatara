#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "pattern/tuple.h"

#include "ast/int.h"

#include "pattern/value_constraint.h"
#include "pattern/wildcard.h"

#include "gtest/gtest.h"

using namespace Pattern;
using namespace std;

TEST(PatternTest, TupleJSON) {
    stringstream                      ss;
    yy::location                      loc;
    auto                              value = make_unique<AST::Int>(loc, 123);
    vector<unique_ptr<class Pattern>> patterns;
    patterns.emplace_back(make_unique<ValueConstraint>(loc, move(value)));
    patterns.emplace_back(make_unique<Wildcard>(loc));
    Tuple node(loc, move(patterns));
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

    ostringstream walk;
    node.walk([&walk](const AST::Node &n) { walk << n.getNodeName() << endl; });
    EXPECT_EQ(walk.str(), "Tuple Pattern\nValue Constraint Pattern\nInt\nWildcard Pattern\n");
}

TEST(PatternTest, TupleWalk) {
    stringstream                      ss;
    yy::location                      loc;
    auto                              value = make_unique<AST::Int>(loc, 123);
    vector<unique_ptr<class Pattern>> patterns;
    patterns.emplace_back(make_unique<ValueConstraint>(loc, move(value)));
    patterns.emplace_back(make_unique<Wildcard>(loc));
    Tuple node(loc, move(patterns));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Tuple Pattern\nValue Constraint Pattern\nInt\nWildcard Pattern\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
