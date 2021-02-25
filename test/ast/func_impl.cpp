#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/func_impl.h"

#include "ast/block.h"
#include "ast/variable.h"

#include "pattern/named_constraint.h"
#include "pattern/type_constraint.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, FuncImplJSON) {
    ostringstream                        ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_unique<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto     block = make_unique<Block>(loc, move(stmts));
    FuncImpl node(loc, loc, name, move(args), move(block), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function impl",)"
        R"("variable":"foo",)"
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

TEST(ASTTest, FuncImplWalk) {
    ostringstream                        ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_unique<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto     block = make_unique<Block>(loc, move(stmts));
    FuncImpl node(loc, loc, name, move(args), move(block), move(ret));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(
        ss.str(),
        "Func Impl\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\nObject Type\n"
        "Block\nVariable\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
