#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/func.h"

#include "ast/block.h"
#include "ast/variable.h"

#include "pattern/named_constraint.h"
#include "pattern/type_constraint.h"

#include "type/func.h"
#include "type/object.h"
#include "type/type_exception.h"
#include "type/unit.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, FuncImplJSON) {
    ostringstream                        ss;
    yy::location                         loc;
    string                               name = "foo";
    vector<unique_ptr<Pattern::Pattern>> args;
    auto                                 argType = make_shared<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_shared<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto block = make_unique<Block>(loc, move(stmts));
    Func node(loc, loc, name, move(args), move(block), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function",)"
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
    auto                                 argType = make_shared<TypeChecker::Object>(loc, "S");
    auto typeConstraint = make_unique<Pattern::TypeConstraint>(loc, move(argType));
    args.emplace_back(make_unique<Pattern::NamedConstraint>(loc, "arg", move(typeConstraint)));
    auto                           ret = make_shared<TypeChecker::Object>(loc, "T");
    vector<unique_ptr<Expression>> stmts;
    stmts.emplace_back(make_unique<Variable>(loc, "b"));
    auto block = make_unique<Block>(loc, move(stmts));
    Func node(loc, loc, name, move(args), move(block), move(ret));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(
        ss.str(),
        "Func Impl\nNamed Constraint Pattern\nType Constraint Pattern\nObject Type\nObject Type\n"
        "Block\nVariable\n");
}

TEST(ASTTest, FuncGetType) {
    istringstream                 iss("func a(x:float): int -> 123");
    shared_ptr<TypeChecker::Type> argType =
                                      make_shared<TypeChecker::Object>(yy::location{}, "float"),
                                  retType = make_shared<TypeChecker::Object>(yy::location{}, "int");
    auto target = make_shared<TypeChecker::Func>(yy::location{}, move(argType), move(retType));
    EXPECT_TYPE(iss, "a", target);
}

TEST(ASTTest, FuncGetTypeWrongReturn) {
    istringstream iss("func a(x:float): bool -> 123");
    SETUP_SCANNER((iss));
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    TypeChecker::Context ctx;
    EXPECT_THROW(
        {
            try {
                for (auto &stmt : drv.statements) {
                    stmt->getType(ctx);
                }
            } catch (const TypeChecker::TypeException &e) {
                auto msgs = e.getMsgs();
                EXPECT_EQ(msgs.size(), 2);
                EXPECT_EQ(
                    msgs[0].first,
                    "returning \"int\" from a function expecting to return \"bool\"");
                EXPECT_EQ(msgs[1].first, "function given type \"bool\" here:");
                throw;
            }
        },
        TypeChecker::TypeException);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
