#include "gtest/gtest.h"
#include "type/object.h"

TEST(TypeTest, ObjectJSON) {
    yy::location        loc;
    TypeChecker::Object obj(loc, "class_name");
    std::stringstream   ss;
    ss << obj;
    EXPECT_EQ(ss.str(), R"({"type":"object","class":"class_name"})");
}
