#include "gtest/gtest.h"
// See https://github.com/google/googletest/blob/master/googletest/docs/primer.md
// for fuller assert documentation
TEST(ExampleSuite, ExampleTest) {
    // Assert will fail and stop running the function
    ASSERT_TRUE(true);
    // Expect will fail, but will not stop running the function, both signify test failure
    EXPECT_TRUE(true);
    // Use expect when the rest of the function is independent of the value

    // Binary comparison
    EXPECT_EQ(2, 1 + 1);
    EXPECT_NE(2, 1);
    EXPECT_GT(5, 1);
    // etc...

    // C-string comparison
    ASSERT_STREQ("foo", "foo");
}
