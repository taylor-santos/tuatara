#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include <memory>

#include "type/array.h"
#include "type/func.h"
#include "type/object.h"
#include "type/product.h"
#include "type/sum.h"

#include "gtest/gtest.h"

using namespace std;

TEST(TypeTest, ObjectJSON) {
    yy::location        loc;
    TypeChecker::Object obj(loc, "class_name");
    std::stringstream   ss;
    ss << obj;
    EXPECT_EQ(ss.str(), R"({"kind":"object","class":"class_name"})");
}

TEST(TypeTest, ArrayJSON) {
    yy::location       loc;
    auto               obj = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Array arr(loc, move(obj));
    std::stringstream  ss;
    ss << arr;
    EXPECT_EQ(ss.str(), R"({"kind":"array","type":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, ProductJSON) {
    yy::location           loc;
    TypeChecker::Type::Vec types;
    types.reserve(2);
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "S"));
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "T"));
    TypeChecker::Product arr(loc, move(types));
    std::stringstream    ss;
    ss << arr;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"product",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");
}

TEST(TypeTest, SumJSON) {
    yy::location           loc;
    TypeChecker::Type::Vec types;
    types.reserve(2);
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "S"));
    types.emplace_back(make_unique<TypeChecker::Object>(loc, "T"));
    TypeChecker::Sum  arr(loc, move(types));
    std::stringstream ss;
    ss << arr;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"sum",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");
}

TEST(TypeTest, NullaryVoidFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg, ret;
    TypeChecker::Func                func(loc, move(arg), move(ret));
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func"})");
}

TEST(TypeTest, NullaryFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg, ret = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func                func(loc, move(arg), move(ret));
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func","return type":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, VoidFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> ret, arg = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func                func(loc, move(arg), move(ret));
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func","arg":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, FuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg = make_unique<TypeChecker::Object>(loc, "S"),
                                     ret = make_unique<TypeChecker::Object>(loc, "T");
    TypeChecker::Func func(loc, move(arg), move(ret));
    std::stringstream ss;
    ss << func;
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

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
