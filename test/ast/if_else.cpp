#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/if_else.h"

#include "ast/bool.h"
#include "ast/float.h"
#include "ast/int.h"

#include "type/sum.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, IfElseJSON) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> trueStmts;
    trueStmts.emplace_back(make_unique<Int>(loc, 123));
    auto                           trueBlock = make_unique<Block>(loc, move(trueStmts));
    vector<unique_ptr<Expression>> falseStmts;
    falseStmts.emplace_back(make_unique<Float>(loc, 4.56));
    auto   falseBlock = make_unique<Block>(loc, move(falseStmts));
    IfElse node(loc, move(cond), move(trueBlock), move(falseBlock));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"if else",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]},)"
        R"("if false":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"float",)"
        R"("value":4.56}]}})");
}

TEST(ASTTest, IfElseWalk) {
    ostringstream                  ss;
    yy::location                   loc;
    auto                           cond = make_unique<Bool>(loc, true);
    vector<unique_ptr<Expression>> trueStmts;
    trueStmts.emplace_back(make_unique<Int>(loc, 123));
    auto                           trueBlock = make_unique<Block>(loc, move(trueStmts));
    vector<unique_ptr<Expression>> falseStmts;
    falseStmts.emplace_back(make_unique<Float>(loc, 4.56));
    auto   falseBlock = make_unique<Block>(loc, move(falseStmts));
    IfElse node(loc, move(cond), move(trueBlock), move(falseBlock));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "If Else\nBool\nBlock\nInt\nBlock\nFloat\n");
}

TEST(ASTTest, IfElseGetType) {
    istringstream                 iss("var a = if true then 123 else 456");
    shared_ptr<TypeChecker::Type> target = make_unique<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, IfElseGetTypeSumType) {
    istringstream                         iss("var a = if true then 123 else 4.56");
    vector<shared_ptr<TypeChecker::Type>> types;
    types.emplace_back(make_shared<TypeChecker::Object>(yy::location{}, "int"));
    types.emplace_back(make_shared<TypeChecker::Object>(yy::location{}, "float"));
    shared_ptr<TypeChecker::Type> target =
        make_unique<TypeChecker::Sum>(yy::location{}, move(types));
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
