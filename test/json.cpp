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

TEST(JSONTest, ObjectWithOneString) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", "value");
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": \"value\"\n}");
}

TEST(JSONTest, MinimizedObjectWithOneString) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", "value");
    }
    EXPECT_EQ(ss.str(), R"({"key":"value"})");
}

TEST(JSONTest, ObjectWithOneInt) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", 123);
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": 123\n}");
}

TEST(JSONTest, MinimizedObjectWithOneInt) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", 123);
    }
    EXPECT_EQ(ss.str(), R"({"key":123})");
}

TEST(JSONTest, ObjectWithOneFloat) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", 123.456);
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": 123.456\n}");
}

TEST(JSONTest, MinimizedObjectWithOneFloat) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("key", 123.456);
    }
    EXPECT_EQ(ss.str(), R"({"key":123.456})");
}

TEST(JSONTest, ObjectWithTwoStrings) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", "first_value");
        obj.KeyValue("second_key", "second_value");
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": \"first_value\",\n"
        "\t\"second_key\": \"second_value\"\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithTwoStrings) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", "first_value");
        obj.KeyValue("second_key", "second_value");
    }
    EXPECT_EQ(ss.str(), R"({"first_key":"first_value","second_key":"second_value"})");
}

TEST(JSONTest, ObjectWithStringInt) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", "first_value");
        obj.KeyValue("second_key", 123);
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": \"first_value\",\n"
        "\t\"second_key\": 123\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithStringInt) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", "first_value");
        obj.KeyValue("second_key", 123);
    }
    EXPECT_EQ(ss.str(), R"({"first_key":"first_value","second_key":123})");
}

TEST(JSONTest, ObjectWithIntString) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", 123);
        obj.KeyValue("second_key", "second_value");
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": 123,\n"
        "\t\"second_key\": \"second_value\"\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithIntString) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", 123);
        obj.KeyValue("second_key", "second_value");
    }
    EXPECT_EQ(ss.str(), R"({"first_key":123,"second_key":"second_value"})");
}

TEST(JSONTest, ObjectWithTwoInts) {
    std::ostringstream ss;
    JSON::JSON::minimize = false;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", 123);
        obj.KeyValue("second_key", 456);
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": 123,\n"
        "\t\"second_key\": 456\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithTwoInts) {
    std::ostringstream ss;
    JSON::JSON::minimize = true;
    {
        JSON::Object obj(ss);
        obj.KeyValue("first_key", 123);
        obj.KeyValue("second_key", 456);
    }
    EXPECT_EQ(ss.str(), R"({"first_key":123,"second_key":456})");
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
            obj2.KeyValue("key", "value");
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
            obj2.KeyValue("key", "value");
        }
    }
    EXPECT_EQ(ss.str(), R"({"obj":{"key":"value"}})");
}
