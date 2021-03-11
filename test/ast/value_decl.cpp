#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/int.h"
#include "ast/value_declaration.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, ValueDeclJSON) {
    ostringstream    ss;
    yy::location     loc;
    auto             val = make_unique<Int>(loc, 123);
    ValueDeclaration node(loc, loc, "var", move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"value declaration",)"
        R"("variable":"var",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, ValueDeclWalk) {
    ostringstream    ss;
    yy::location     loc;
    auto             val = make_unique<Int>(loc, 123);
    ValueDeclaration node(loc, loc, "var", move(val));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Value Decl\nInt\n");
}

TEST(ASTTest, ValueDeclGetType) {
    istringstream iss(R"(var a = 123)");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
