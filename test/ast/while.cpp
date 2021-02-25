#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/while.h"

#include "ast/block.h"
#include "ast/bool.h"
#include "ast/int.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, WhileJSON) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto  block = make_unique<Block>(loc, move(stmts));
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
}

TEST(ASTTest, WhileWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto  block = make_unique<Block>(loc, move(stmts));
    While node(loc, move(cond), move(block));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "While\nBool\nBlock\nInt\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
