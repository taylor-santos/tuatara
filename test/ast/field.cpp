#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "ast/field.h"

#include "ast/variable.h"

#include "type/func.h"
#include "type/object.h"

#include "test_util.h"

using namespace AST;
using namespace std;

TEST(ASTTest, FieldJSON) {
    ostringstream ss;
    yy::location  loc;
    Field         node(loc, make_unique<Variable>(loc, "a"), loc, "b");
    node.walk([&ss](const Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Field\nVariable\n");
}

TEST(ASTTest, FieldWalk) {
    ostringstream ss;
    yy::location  loc;
    Field         node(loc, make_unique<Variable>(loc, "a"), loc, "b");
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"b"})");
}

TEST(ASTTest, FieldGetType) {
    istringstream iss("var someInt = 123\n"
                      "var a = someInt.+");

    yy::location                  loc;
    shared_ptr<TypeChecker::Type> argType = make_shared<TypeChecker::Object>(loc, "int"),
                                  retType = make_shared<TypeChecker::Object>(loc, "int");
    auto target = make_shared<TypeChecker::Func>(loc, move(argType), move(retType));
    EXPECT_TYPE(iss, "a", target);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
