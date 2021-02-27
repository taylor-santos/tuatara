#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/lambda.h"

#include "ast/block.h"
#include "ast/variable.h"

#include "pattern/named_constraint.h"
#include "pattern/type_constraint.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, LambdaJSON) {
    ostringstream                        ss;
    yy::location                         loc;
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_shared<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_shared<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto   block = make_unique<Block>(loc, move(stmts));
    Lambda node(loc, move(args), move(ret), move(block));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"arg",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"S"}}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"},)"
        R"("body":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"b"}]}})");
}

TEST(ASTTest, LambdaWalk) {
    ostringstream                        ss;
    yy::location                         loc;
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_shared<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_shared<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto   block = make_unique<Block>(loc, move(stmts));
    Lambda node(loc, move(args), move(ret), move(block));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(
        ss.str(),
        "Lambda\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\n"
        "Object Type\nBlock\nVariable\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
