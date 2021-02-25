#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "json.h"

#include <sstream>

#include "gtest/gtest.h"

using namespace std;

TEST(JSONTest, EmptyObject) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    { JSON::Object obj(ss); }
    EXPECT_EQ(ss.str(), "{}");
}

TEST(JSONTest, MinimizedEmptyObject) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    { JSON::Object obj(ss); }
    EXPECT_EQ(ss.str(), "{}");
}

TEST(JSONTest, EmptyArray) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    { JSON::Array arr(ss); }
    EXPECT_EQ(ss.str(), "[]");
}

TEST(JSONTest, MinimizedEmptyArray) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    { JSON::Array arr(ss); }
    EXPECT_EQ(ss.str(), "[]");
}

TEST(JSONTest, ObjectWithOneString) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", "value");
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": \"value\"\n}");
}

TEST(JSONTest, MinimizedObjectWithOneString) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", "value");
    }
    EXPECT_EQ(ss.str(), R"({"key":"value"})");
}

TEST(JSONTest, ObjectWithOneInt) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", 123);
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": 123\n}");
}

TEST(JSONTest, MinimizedObjectWithOneInt) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", 123);
    }
    EXPECT_EQ(ss.str(), R"({"key":123})");
}

TEST(JSONTest, ObjectWithOneFloat) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", 123.456);
    }
    EXPECT_EQ(ss.str(), "{\n\t\"key\": 123.456\n}");
}

TEST(JSONTest, MinimizedObjectWithOneFloat) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("key", 123.456);
    }
    EXPECT_EQ(ss.str(), R"({"key":123.456})");
}

TEST(JSONTest, ObjectWithTwoStrings) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", "first_value");
        obj.printKeyValue("second_key", "second_value");
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": \"first_value\",\n"
        "\t\"second_key\": \"second_value\"\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithTwoStrings) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", "first_value");
        obj.printKeyValue("second_key", "second_value");
    }
    EXPECT_EQ(ss.str(), R"({"first_key":"first_value","second_key":"second_value"})");
}

TEST(JSONTest, ObjectWithStringInt) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", "first_value");
        obj.printKeyValue("second_key", 123);
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": \"first_value\",\n"
        "\t\"second_key\": 123\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithStringInt) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", "first_value");
        obj.printKeyValue("second_key", 123);
    }
    EXPECT_EQ(ss.str(), R"({"first_key":"first_value","second_key":123})");
}

TEST(JSONTest, ObjectWithIntString) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", 123);
        obj.printKeyValue("second_key", "second_value");
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": 123,\n"
        "\t\"second_key\": \"second_value\"\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithIntString) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", 123);
        obj.printKeyValue("second_key", "second_value");
    }
    EXPECT_EQ(ss.str(), R"({"first_key":123,"second_key":"second_value"})");
}

TEST(JSONTest, ObjectWithTwoInts) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", 123);
        obj.printKeyValue("second_key", 456);
    }
    EXPECT_EQ(
        ss.str(),
        "{\n"
        "\t\"first_key\": 123,\n"
        "\t\"second_key\": 456\n"
        "}");
}

TEST(JSONTest, MinimizedObjectWithTwoInts) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKeyValue("first_key", 123);
        obj.printKeyValue("second_key", 456);
    }
    EXPECT_EQ(ss.str(), R"({"first_key":123,"second_key":456})");
}

TEST(JSONTest, ArrayWithOneString) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Array arr(ss);
        arr.printValue("test");
    }
    EXPECT_EQ(
        ss.str(),
        "[\n"
        "\t\"test\"\n"
        "]");
}

TEST(JSONTest, MinimizedArrayWithOneString) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Array arr(ss);
        arr.printValue("test");
    }
    EXPECT_EQ(ss.str(), R"(["test"])");
}

TEST(JSONTest, ArrayWithTwoStrings) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Array arr(ss);
        arr.printValue("first_string");
        arr.printValue("second_string");
    }
    EXPECT_EQ(
        ss.str(),
        "[\n"
        "\t\"first_string\",\n"
        "\t\"second_string\"\n"
        "]");
}

TEST(JSONTest, MinimizedArrayWithTwoStrings) {
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Array arr(ss);
        arr.printValue("first_string");
        arr.printValue("second_string");
    }
    EXPECT_EQ(ss.str(), R"(["first_string","second_string"])");
}

TEST(JSONTest, NestedObjects) {
    ostringstream ss;
    JSON::JSON::setMinimize(false);
    {
        JSON::Object obj(ss);
        obj.printKey("obj");
        {
            JSON::Object obj2(ss);
            obj2.printKeyValue("key", "value");
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
    ostringstream ss;
    JSON::JSON::setMinimize(true);
    {
        JSON::Object obj(ss);
        obj.printKey("obj");
        {
            JSON::Object obj2(ss);
            obj2.printKeyValue("key", "value");
        }
    }
    EXPECT_EQ(ss.str(), R"({"obj":{"key":"value"}})");
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
