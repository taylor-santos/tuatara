#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/func_declaration.h"

#include "pattern/named_constraint.h"
#include "pattern/type_constraint.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, FuncDeclJSON) {
    ostringstream                        ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto            ret = make_unique<TypeChecker::Object>(loc, "T");
    FuncDeclaration node(loc, loc, name, move(args), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function declaration",)"
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
        R"("class":"T"}})");
}

TEST(ASTTest, FuncDeclarationNode) {
    ostringstream                        ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_unique<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto            ret = make_unique<TypeChecker::Object>(loc, "T");
    FuncDeclaration node(loc, loc, name, move(args), move(ret));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(
        ss.str(),
        "Func Decl\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
