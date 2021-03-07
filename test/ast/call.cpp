#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/call.h"

#include "ast/variable.h"

#include "type/object.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, CallJSON) {
    ostringstream ss;
    yy::location  loc;
    auto          func = make_unique<Variable>(loc, "a");
    auto          arg  = make_unique<Variable>(loc, "b");
    Call          node(loc, move(func), move(arg));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("arg":{)"
        R"("node":"variable",)"
        R"("name":"b"}})");
}

TEST(ASTTest, CallWalk) {
    ostringstream ss;
    yy::location  loc;
    auto          func = make_unique<Variable>(loc, "a");
    auto          arg  = make_unique<Variable>(loc, "b");
    Call          node(loc, move(func), move(arg));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Call\nVariable\nVariable\n");
}

TEST(ASTTest, CallGetType) {
    istringstream iss("func fn() -> 123\n"
                      "var a = fn()");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
