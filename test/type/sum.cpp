#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/sum.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, SumJSON) {
    stringstream        ss;
    yy::location             loc;
    vector<unique_ptr<Type>> types;
    types.reserve(2);
    types.emplace_back(make_unique<Object>(loc, "S"));
    types.emplace_back(make_unique<Object>(loc, "T"));
    Sum node(loc, move(types));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"sum",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");
}

TEST(TypeTest, SumWalk) {
    stringstream        ss;
    yy::location             loc;
    vector<unique_ptr<Type>> types;
    types.reserve(2);
    types.emplace_back(make_unique<Object>(loc, "S"));
    types.emplace_back(make_unique<Object>(loc, "T"));
    Sum node(loc, move(types));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Sum Type\nObject Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
