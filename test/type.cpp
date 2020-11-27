#include "gtest/gtest.h"
#include "type/object.h"
#include "type/array.h"
#include "type/tuple.h"
#include "type/func.h"

#include <memory>

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
    auto               obj = make_shared<TypeChecker::Object>(loc, "T");
    TypeChecker::Array arr(loc, obj);
    std::stringstream  ss;
    ss << arr;
    EXPECT_EQ(ss.str(), R"({"kind":"array","type":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, TupleJSON) {
    yy::location           loc;
    TypeChecker::Type::Vec types;
    types.reserve(2);
    types.emplace_back(make_shared<TypeChecker::Object>(loc, "S"));
    types.emplace_back(make_shared<TypeChecker::Object>(loc, "T"));
    TypeChecker::Tuple arr(loc, types);
    std::stringstream  ss;
    ss << arr;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"tuple",)"
        R"("types":[)"
        R"({"kind":"object","class":"S"},)"
        R"({"kind":"object","class":"T"}]})");
}

TEST(TypeTest, NullaryVoidFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg, ret;
    TypeChecker::Func                func(loc, arg, ret);
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func"})");
}

TEST(TypeTest, NullaryFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg, ret = make_shared<TypeChecker::Object>(loc, "T");
    TypeChecker::Func                func(loc, arg, ret);
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func","returns":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, VoidFuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> ret, arg = make_shared<TypeChecker::Object>(loc, "T");
    TypeChecker::Func                func(loc, arg, ret);
    std::stringstream                ss;
    ss << func;
    EXPECT_EQ(ss.str(), R"({"kind":"func","arg":{"kind":"object","class":"T"}})");
}

TEST(TypeTest, FuncJSON) {
    yy::location                     loc;
    optional<TypeChecker::Type::Ptr> arg = make_shared<TypeChecker::Object>(loc, "S"),
                                     ret = make_shared<TypeChecker::Object>(loc, "T");
    TypeChecker::Func func(loc, arg, ret);
    std::stringstream ss;
    ss << func;
    EXPECT_EQ(
        ss.str(),
        R"({"kind":"func","arg":{"kind":"object","class":"S"},"returns":{"kind":"object","class":"T"}})");
}