#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "type/func.h"

#include "type/object.h"

#include "gtest/gtest.h"

using namespace TypeChecker;
using namespace std;

TEST(TypeTest, FuncJSON) {
    stringstream ss;
    yy::location      loc;
    auto              arg = make_unique<Object>(loc, "S");
    auto              ret = make_unique<Object>(loc, "T");
    Func              node(loc, move(arg), move(ret));
    ss << node;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"S"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"}})");
}

TEST(TypeTest, FuncWalk) {
    stringstream ss;
    yy::location      loc;
    auto              arg = make_unique<Object>(loc, "S");
    auto              ret = make_unique<Object>(loc, "T");
    Func              node(loc, move(arg), move(ret));
    node.walk([&ss](const AST::Node &n) { ss << n.getNodeName() << endl; });
    EXPECT_EQ(ss.str(), "Func Type\nObject Type\nObject Type\n");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
