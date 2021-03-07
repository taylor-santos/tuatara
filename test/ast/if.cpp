#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/if.h"

#include "ast/bool.h"
#include "ast/int.h"

#include "type/sum.h"
#include "type/unit.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IfJSON) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto block = make_unique<Block>(loc, move(stmts));
    If   node(loc, move(cond), move(block));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]}})");
}

TEST(ASTTest, IfWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Int>(loc, 123));
    auto block = make_unique<Block>(loc, move(stmts));
    If   node(loc, move(cond), move(block));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "If\nBool\nBlock\nInt\n");
}

TEST(ASTTest, IfGetType) {
    istringstream                         iss("var a = if true then 123");
    vector<shared_ptr<TypeChecker::Type>> types;
    types.emplace_back(make_shared<TypeChecker::Object>(yy::location{}, "int"));
    types.emplace_back(make_shared<TypeChecker::Unit>(yy::location{}));
    shared_ptr<TypeChecker::Type> target =
        make_unique<TypeChecker::Sum>(yy::location{}, move(types));
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
