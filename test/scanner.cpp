#include "gtest/gtest.h"
#include "driver.h"
#include "scanner.h"

#include <cmath>

TEST(ScannerTest, Whitespace) {
    yy::Scanner        scan;
    std::istringstream iss("\t \n \t \r\n\t \t\t  ");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Int) {
    yy::Scanner       scan;
    int64_t           max = std::numeric_limits<int64_t>::max();
    std::stringstream ss;
    int64_t           expected[] = {0, 1, 12, 123, max};
    ss << "                         0  1  12  123 " << max;
    std::ostringstream oss;
    scan.switch_streams(ss, oss);
    yy::Driver drv;
    for (auto expect : expected) {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<int64_t>();
        EXPECT_EQ(expect, got);
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, IntOverflow) {
    yy::Scanner       scan;
    uint64_t          max = std::numeric_limits<int64_t>::max();
    std::stringstream ss;
    ss << max + 1;
    std::ostringstream oss;
    scan.switch_streams(ss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                scan.yylex(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_EQ(e.what(), "int is out of range: " + std::to_string(max + 1));
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Float) {
    yy::Scanner scan;
    double      max        = std::numeric_limits<double>::max();
    double      min        = std::numeric_limits<double>::min();
    double      expected[] = {
        .1,      .12,    1.,      1.2,      1.23,   12.,     12.3,     12.34,     1e2,   .1e2,
        .12e3,   1.e2,   1.2e3,   1.23e4,   12e3,   12.e3,   12.3e4,   12.34e5,   1e+2,  .1e+2,
        .12e+3,  1.e+2,  1.2e+3,  1.23e+4,  12e+3,  12.e+3,  12.3e+4,  12.34e+5,  1e-2,  .1e-2,
        .12e-3,  1.e-2,  1.2e-3,  1.23e-4,  12e-3,  12.e-3,  12.3e-4,  12.34e-5,  1e23,  .1e23,
        .12e34,  1.e23,  1.2e34,  1.23e45,  12e34,  12.e34,  12.3e45,  12.34e56,  1e+23, .1e+23,
        .12e+34, 1.e+23, 1.2e+34, 1.23e+45, 12e+34, 12.e+34, 12.3e+45, 12.34e+56, 1e-23, .1e-23,
        .12e-34, 1.e-23, 1.2e-34, 1.23e-45, 12e-34, 12.e-34, 12.3e-45, 12.34e-56, max,   min};
    std::stringstream iss;
    iss << R"(
        .1       .12     1.       1.2       1.23    12.      12.3      12.34      1e2    .1e2
        .12e3    1.e2    1.2e3    1.23e4    12e3    12.e3    12.3e4    12.34e5    1e+2   .1e+2
        .12e+3   1.e+2   1.2e+3   1.23e+4   12e+3   12.e+3   12.3e+4   12.34e+5   1e-2   .1e-2
        .12e-3   1.e-2   1.2e-3   1.23e-4   12e-3   12.e-3   12.3e-4   12.34e-5   1e23   .1e23
        .12e34   1.e23   1.2e34   1.23e45   12e34   12.e34   12.3e45   12.34e56   1e+23  .1e+23
        .12e+34  1.e+23  1.2e+34  1.23e+45  12e+34  12.e+34  12.3e+45  12.34e+56  1e-23  .1e-23
        .12e-34  1.e-23  1.2e-34  1.23e-45  12e-34  12.e-34  12.3e-45  12.34e-56 )";
    iss << std::setprecision(20) << max << " " << min;
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    for (auto expect : expected) {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<double>();
        EXPECT_DOUBLE_EQ(expect, got);
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NaN) {
    yy::Scanner        scan;
    int                example_count = 8;
    std::istringstream iss(R"(nan Nan nAn NAn naN NaN nAN NAN)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    for (int i = 0; i < example_count; i++) {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<double>();
        EXPECT_TRUE(std::isnan(got));
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Infinity) {
    yy::Scanner        scan;
    int                example_count = 8;
    std::istringstream iss(R"(inf Inf iNf INf inF InF iNF INF)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    for (int i = 0; i < example_count; i++) {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<double>();
        EXPECT_TRUE(std::isinf(got));
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Comment) {
    yy::Scanner        scan;
    std::istringstream iss(R"(// Comments should be ignored up to the line break
                              123 // Everything before a comment should be matched
                              // Comments at end of source should be ignored too)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<int64_t>();
        EXPECT_EQ(got, 123);
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockComment) {
    yy::Scanner        scan;
    std::istringstream iss(R"(/* Block comments can span
                                 multiple lines. */)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockCommentsMustClose) {
    yy::Scanner        scan;
    std::istringstream iss(R"(/* Block comments must be closed.
                                 If the file ends inside a block comment,
                                 it is an error.)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scan.yylex(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected EOF in block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NestedBlockComment) {
    yy::Scanner        scan;
    std::istringstream iss(R"(/* Block comments can be
                              /* nested */ but every
                                 comment opening /* needs to be matched
                                 by an equivalent closing */ */)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NestedBlockCommentsMustClose) {
    yy::Scanner        scan;
    std::istringstream iss(R"(/* Nested Block comments must also be closed.
                              /* Even if the inner block is closed, */
                                 failing to close the outer block by the end of
                                 the file is an error.)");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scan.yylex(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected EOF in block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, MisplacedBlockCommentClose) {
    yy::Scanner        scan;
    std::istringstream iss(R"(  */  )");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scan.yylex(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected */ outside of block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NoLeadingUnderscoresInIdent) {
    yy::Scanner        scan;
    std::istringstream iss("_NoLeadingUnderscores");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                scan.yylex(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "invalid character: '_'");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
}

TEST(ScannerTest, EmptyStringLiteral) {
    yy::Scanner        scan;
    std::istringstream iss(R"("")");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<std::string>();
        EXPECT_EQ(got, "");
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, StringLiteral) {
    yy::Scanner        scan;
    std::istringstream iss(R"("foo bar")");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<std::string>();
        EXPECT_EQ(got, "foo bar");
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, StringLiteralWithEscapedQuote) {
    yy::Scanner        scan;
    std::istringstream iss(R"("foo\"bar")");
    std::ostringstream oss;
    scan.switch_streams(iss, oss);
    yy::Driver drv;
    {
        auto tok = scan.yylex(drv);
        auto got = tok.value.as<std::string>();
        EXPECT_EQ(got, R"(foo\"bar)");
    }
    auto tok = scan.yylex(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}
