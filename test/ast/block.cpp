#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/block.h"

#include "ast/float.h"
#include "ast/int.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, BlockJSON) {
    ostringstream                  ss;
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
}

TEST(ASTTest, BlockWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    stmts.emplace_back(make_unique<Float>(loc, 45.6));
    Block node(loc, move(stmts));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Block\nInt\nFloat\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
