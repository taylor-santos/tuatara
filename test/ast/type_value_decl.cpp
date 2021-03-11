#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/int.h"
#include "ast/type_value_declaration.h"

#include "type/object.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, TypeValueDeclJSON) {
    ostringstream        ss;
    yy::location         loc;
    auto                 type = make_shared<TypeChecker::Object>(loc, "class_name");
    auto                 val  = make_unique<Int>(loc, 123);
    TypeValueDeclaration node(loc, loc, "var", move(type), move(val));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"type value declaration",)"
        R"("variable":"var",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"class_name"},)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ASTTest, TypeValueDeclWalk) {
    ostringstream        ss;
    yy::location         loc;
    auto                 type = make_shared<TypeChecker::Object>(loc, "class_name");
    auto                 val  = make_unique<Int>(loc, 123);
    TypeValueDeclaration node(loc, loc, "var", move(type), move(val));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Type Value Decl\nObject Type\nInt\n");
}

TEST(ASTTest, TypeValueDeclGetType) {
    istringstream iss(R"(var a: int = 123)");

    auto target = make_shared<TypeChecker::Object>(yy::location{}, "int");
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
