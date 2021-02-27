#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/product.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, ProductJSON) {
    yy::location             loc;
    vector<shared_ptr<Type>> types;
    types.reserve(2);
    types.emplace_back(make_shared<Object>(loc, "S"));
    types.emplace_back(make_shared<Object>(loc, "T"));
    Product      node(loc, move(types));
    stringstream ss;
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"product",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");
}

TEST(TypeTest, ProductWalk) {
    stringstream             ss;
    yy::location             loc;
    vector<shared_ptr<Type>> types;
    types.reserve(2);
    types.emplace_back(make_shared<Object>(loc, "S"));
    types.emplace_back(make_shared<Object>(loc, "T"));
    Product node(loc, move(types));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Product Type\nObject Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
