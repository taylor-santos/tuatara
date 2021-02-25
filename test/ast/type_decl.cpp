#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/type_declaration.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace AST;
using namespace std;

TEST(ASTTest, TypeDeclJSON) {
    ostringstream   ss;
    yy::location    loc;
    auto            type = make_unique<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration node(loc, loc, "var", move(type));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"type declaration",)"
        R"("variable":"var",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"class_name"}})");
}

TEST(ASTTest, TypeDeclWalk) {
    ostringstream   ss;
    yy::location    loc;
    auto            type = make_unique<TypeChecker::Object>(loc, "class_name");
    TypeDeclaration node(loc, loc, "var", move(type));
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Type Decl\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
