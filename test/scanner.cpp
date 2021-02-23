#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "scanner.h"

#include <cmath>

#include "driver.h"
#include "gtest/gtest.h"
#include "macro_map.h"

#define STR(x) #x

// Define the sample data as a macro so we can automatically construct matching float and string
// arrays, to test whether C++'s scanner interprets each float in the same way that ours does.
#define INT_SCAN_SAMPLES 0, 1, 12, 123, 55555
#define FLOAT_SCAN_SAMPLES                                                                        \
    .1, .12, 1., 1.2, 1.23, 12., 12.3, 12.34, 1e2, .1e2, .12e3, 1.e2, 1.2e3, 1.23e4, 12e3, 12.e3, \
        12.3e4, 12.34e5, 1e+2, .1e+2, .12e+3, 1.e+2, 1.2e+3, 1.23e+4, 12e+3, 12.e+3, 12.3e+4,     \
        12.34e+5, 1e-2, .1e-2, .12e-3, 1.e-2, 1.2e-3, 1.23e-4, 12e-3, 12.e-3, 12.3e-4, 12.34e-5,  \
        1e23, .1e23, .12e34, 1.e23, 1.2e34, 1.23e45, 12e34, 12.e34, 12.3e45, 12.34e56, 1e+23,     \
        .1e+23, .12e+34, 1.e+23, 1.2e+34, 1.23e+45, 12e+34, 12.e+34, 12.3e+45, 12.34e+56, 1e-23,  \
        .1e-23, .12e-34, 1.e-23, 1.2e-34, 1.23e-45, 12e-34, 12.e-34, 12.3e-45, 12.34e-56

#define EXPECT_NO_ASSERT(statement)   \
    EXPECT_EXIT(                      \
        {                             \
            statement;                \
            exit(0);                  \
        },                            \
        ::testing::ExitedWithCode(0), \
        ".*")

#define EXPECT_VALUE_TOK(SCANNER, DRIVER, TYPE, TOKEN, VALUE)                                      \
    do {                                                                                           \
        auto tok = SCANNER.scan(DRIVER);                                                           \
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_##TOKEN)                         \
            << "expected a \"" << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_##TOKEN) \
            << "\" but got a \"" << tok.name() << "\"";                                            \
        auto got = tok.value.as<TYPE>();                                                           \
        EXPECT_EQ(got, VALUE) << "expected value \"" << VALUE << "\" but got \"" << got << "\"";   \
    } while (0)

#define EXPECT_TOK(SCANNER, DRIVER, TOKEN)                                                         \
    do {                                                                                           \
        auto tok = SCANNER.scan(DRIVER);                                                           \
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_##TOKEN)                         \
            << "expected a \"" << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_##TOKEN) \
            << "\" but got a \"" << tok.name() << "\"";                                            \
    } while (0)

#define EXPECT_EOF(SCANNER, DRIVER)           \
    EXPECT_NO_THROW({                         \
        EXPECT_TOK(SCANNER, DRIVER, NEWLINE); \
        EXPECT_TOK(SCANNER, DRIVER, YYEOF);   \
    })

TEST(ScannerTest, Whitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("\t \n \t \r\n\t \t\t  ");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Int) {
    yy::Scanner       scanner;
    int64_t           max = std::numeric_limits<int64_t>::max();
    std::stringstream iss;
    int64_t           expected[] = {INT_SCAN_SAMPLES, max};           // Insert the data as floats
    auto              samples    = {MAP_LIST(STR, INT_SCAN_SAMPLES)}; // Convert the data to strings
    for (const auto &s : samples) {
        iss << s << " ";
    }
    iss << std::setprecision(20) << max;
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    for (const auto &expect : expected) {
        EXPECT_VALUE_TOK(scanner, drv, int64_t, INT, expect);
    }
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, IntOverflow) {
    yy::Scanner       scanner;
    uint64_t          max = std::numeric_limits<int64_t>::max();
    std::stringstream ss;
    ss << max + 1;
    std::ostringstream oss;
    scanner.switch_streams(ss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_EQ(e.what(), "int is out of range: " + std::to_string(max + 1));
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    auto tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Float) {
    yy::Scanner       scanner;
    double            max = std::numeric_limits<double>::max();
    double            min = std::numeric_limits<double>::min();
    std::stringstream iss;
    double            expected[] = {FLOAT_SCAN_SAMPLES, max, min};   // Insert the data as floats
    auto              samples = {MAP_LIST(STR, FLOAT_SCAN_SAMPLES)}; // Convert the data to strings
    for (const auto &s : samples) {
        iss << s << " ";
    }
    iss << std::setprecision(20) << max << " " << min;
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    for (const auto &expect : expected) {
        EXPECT_VALUE_TOK(scanner, drv, double, FLOAT, expect);
    }
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, NaN) {
    yy::Scanner        scanner;
    int                exampleCount = 8;
    std::istringstream iss(R"(nan Nan nAn NAn naN NaN nAN NAN)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    for (int i = 0; i < exampleCount; i++) {
        auto tok = scanner.scan(drv);
        auto got = tok.value.as<double>();
        EXPECT_TRUE(std::isnan(got));
    }
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, Infinity) {
    yy::Scanner        scanner;
    int                exampleCount = 8;
    std::istringstream iss(R"(inf Inf iNf INf inF InF iNF INF)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    for (int i = 0; i < exampleCount; i++) {
        auto tok = scanner.scan(drv);
        auto got = tok.value.as<double>();
        EXPECT_TRUE(std::isinf(got));
    }
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, Comment) {
    yy::Scanner        scanner;
    std::istringstream iss("// Comments should be ignored up to the line break\n"
                           "123 // Everything before a comment should be matched\n"
                           "// Comments at end of source should be ignored too");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, int64_t, INT, 123);
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, BlockComment) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(/* Block comments can span
                                 multiple lines. */)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockCommentsMustClose) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(/* Block comments must be closed.
                                 If the file ends inside a block comment,
                                 it is an error.)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected EOF in block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockCommentMustEndLine) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(/* Block comments must be followed by line breaks */ foo)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "block comment */ must be followed by a line break");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, IndependentBlockCommentsDontOutputNewlines) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\n/* block comment on its own line */\n");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, BlockCommentOwnLine) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\n/* block comment on its own line */\n");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, BlockCommentOwnLineEOF) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\n/* block comment on its own line */");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, BlockCommentSameLine) {
    yy::Scanner        scanner;
    std::istringstream iss("foo /* block comment on same line */\n");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, BlockCommentSameLineEOF) {
    yy::Scanner        scanner;
    std::istringstream iss("foo /* block comment on same line */");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, NestedBlockComment) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(/* Block comments can be
                              /* nested */ but every
                                 comment opening /* needs to be matched
                                 by an equivalent closing */ */)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NestedBlockCommentsMustClose) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(/* Nested Block comments must also be closed.
                              /* Even if the inner block is closed, */
                                 failing to close the outer block by the end of
                                 the file is an error.)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected EOF in block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, MisplacedBlockCommentClose) {
    yy::Scanner        scanner;
    std::istringstream iss(R"(  */  )");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "unexpected */ outside of block comment");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NoLeadingUnderscoresInIdent) {
    yy::Scanner        scanner;
    std::istringstream iss("_NoLeadingUnderscores");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    auto       tok = scanner.scan(drv);
    EXPECT_NE(tok.type_get(), yy::Parser::symbol_kind_type::S_IDENT)
        << "Expected leading underscore to not be matched as part of an identifier";
}

TEST(ScannerTest, EmptyStringLiteral) {
    yy::Scanner        scanner;
    std::istringstream iss(R"("")");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, STRING, "");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, StringLiteral) {
    yy::Scanner        scanner;
    std::istringstream iss(R"("foo bar")");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, STRING, "foo bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, StringLiteralWithEscapedQuote) {
    yy::Scanner        scanner;
    std::istringstream iss(R"("foo\"bar")");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, STRING, R"(foo\"bar)");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, EscapedNewline) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\\\nbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, EscapedNewlineWithTrailingWhitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\\ \nbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, NewlineInParensLeadingWitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo(\n  bar\n)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, LPAREN);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_TOK(scanner, drv, RPAREN);
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, NewlineInParens) {
    yy::Scanner        scanner;
    std::istringstream iss("foo(\nbar\n)");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, LPAREN);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_TOK(scanner, drv, RPAREN);
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, MixedTabWithSpaces) {
    yy::Scanner        scanner;
    std::istringstream iss("    \tfoo");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "inconsistent use of tabs and spaces in indentation");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, MixedSpaceWithTabs) {
    yy::Scanner        scanner;
    std::istringstream iss("\t foo");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                auto tok = scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "inconsistent use of tabs and spaces in indentation");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, InconsistentTabs) {
    yy::Scanner        scanner;
    std::istringstream iss("    foo\n\tbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                EXPECT_TOK(scanner, drv, INDENT);
                EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
                EXPECT_TOK(scanner, drv, NEWLINE);
                EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "inconsistent use of tabs and spaces in indentation");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, InconsistentSpaces) {
    yy::Scanner        scanner;
    std::istringstream iss("\tfoo\n    bar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                EXPECT_TOK(scanner, drv, INDENT);
                EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
                EXPECT_TOK(scanner, drv, NEWLINE);
                EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "inconsistent use of tabs and spaces in indentation");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScanerTest, EOFLineBreak) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\n");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScanerTest, EOFNoLineBreak) {
    yy::Scanner        scanner;
    std::istringstream iss("foo");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScanerTest, EOFLineBreakTrailingWhitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo\n    ");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScanerTest, EOFNoLineBreakTrailingWhitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo    ");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, TrailingSemicolon) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;\nbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, NEWLINE);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, TrailingSemicolonWhitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;    \nbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, NEWLINE);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, TrailingSemicolons) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;;\nbar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, NEWLINE);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, SemicolonSeparator) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;bar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, SEMICOLON);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, SemicolonSeparatorWhitespace) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;   bar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, SEMICOLON);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, SemicolonsSeparator) {
    yy::Scanner        scanner;
    std::istringstream iss("foo;;bar");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "foo");
    EXPECT_TOK(scanner, drv, SEMICOLON);
    EXPECT_VALUE_TOK(scanner, drv, std::string, IDENT, "bar");
    EXPECT_EOF(scanner, drv);
}

TEST(ScannerTest, InvalidEscapeCharacters) {
    yy::Scanner        scanner;
    std::string        chars = "abfv";
    std::string        input = "\a\b\f\v";
    std::istringstream iss(input);
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    for (char c : chars) {
        EXPECT_THROW(
            {
                try {
                    scanner.scan(drv);
                } catch (const yy::Parser::syntax_error &e) {
                    std::string msg = "invalid character: '\\"s + c + "'";
                    EXPECT_EQ(e.what(), msg);
                    throw; // Re-throw exception
                }
            },
            yy::Parser::syntax_error);
    }
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, HexEscapeCharacters) {
    yy::Scanner        scanner;
    std::istringstream iss("\x1B");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_THROW(
        {
            try {
                scanner.scan(drv);
            } catch (const yy::Parser::syntax_error &e) {
                EXPECT_STREQ(e.what(), "invalid character: '0x1B'");
                throw; // Re-throw exception
            }
        },
        yy::Parser::syntax_error);
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, SingleCloseParen) {
    yy::Scanner        scanner;
    std::istringstream iss(")");
    std::ostringstream oss;
    scanner.switch_streams(iss, oss);
    yy::Driver drv;
    EXPECT_TOK(scanner, drv, RPAREN);
    EXPECT_EOF(scanner, drv);
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
