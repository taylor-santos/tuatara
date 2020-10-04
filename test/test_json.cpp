#include "gtest/gtest.h"
#include "json.h"

#include <sstream>

TEST(JSONTest, EmptyObject) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    { JSON::Object obj(ss); }
    EXPECT_EQ(ss.str(), "{}");
}

TEST(JSONTest, MinimizedEmptyObject) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    { JSON::Object obj(ss); }
    EXPECT_EQ(ss.str(), "{}");
}

TEST(JSONTest, EmptyArray) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    { JSON::Array arr(ss); }
    EXPECT_EQ(ss.str(), "[]");
}

TEST(JSONTest, MinimizedEmptyArray) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    { JSON::Array arr(ss); }
    EXPECT_EQ(ss.str(), "[]");
}

TEST(JSONTest, ObjectWithOneKeyValue) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.String("key", "value");
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": \"value\"\n}");
}

TEST(JSONTest, MinimizedObjectWithOneKeyValue) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.String("key", "value");
    }
    EXPECT_EQ(ss.str(), R"({"key":"value"})");
}

TEST(JSONTest, ObjectWithTwoKeyValues) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.String("first_key", "first_value");
        obj.String("second_key", "second_value");
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": \"first_value\",\n"
        "\t\"second_key\": \"second_value\"\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithTwoKeyValues) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.String("first_key", "first_value");
        obj.String("second_key", "second_value");
    }
    EXPECT_EQ(ss.str(), R"({"first_key":"first_value","second_key":"second_value"})");
}

TEST(JSONTest, ArrayWithOneString) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Array arr(ss);
        arr.String("test");
    }
    EXPECT_EQ(
        ss.str(),
        "[\n"
        "\t\"test\"\n"
        "]");
}

TEST(JSONTest, MinimizedArrayWithOneString) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Array arr(ss);
        arr.String("test");
    }
    EXPECT_EQ(ss.str(), R"(["test"])");
}

TEST(JSONTest, ArrayWithTwoStrings) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Array arr(ss);
        arr.String("first_string");
        arr.String("second_string");
    }
    EXPECT_EQ(
        ss.str(),
        "[\n"
        "\t\"first_string\",\n"
        "\t\"second_string\"\n"
        "]");
}

TEST(JSONTest, MinimizedArrayWithTwoStrings) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Array arr(ss);
        arr.String("first_string");
        arr.String("second_string");
    }
    EXPECT_EQ(ss.str(), R"(["first_string","second_string"])");
}

TEST(JSONTest, NestedObjects) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.Key("obj");
        {
            JSON::Object obj2(ss);
            obj2.String("key", "value");
        }
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"obj\": {\n"
        "\t\t\"key\": \"value\"\n"
        "\t}\n"
        "}");
}

TEST(JSONTest, MinimizedNestedObjects) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.Key("obj");
        {
            JSON::Object obj2(ss);
            obj2.String("key", "value");
        }
    }
    EXPECT_EQ(ss.str(), R"({"obj":{"key":"value"}})");
}
