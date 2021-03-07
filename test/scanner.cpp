#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#endif

#include "scanner.h"

#include <cmath>

#include "macro_map.h"
#include "test_util.h"

using namespace std;

TEST(ScannerTest, Whitespace) {
    istringstream iss("\t \n \t \r\n\t \t\t  ");
    SETUP_SCANNER(iss);
    auto tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Int) {
#define INT_SCAN_SAMPLES 0, 1, 12, 123, 55555
    int64_t      max        = numeric_limits<int64_t>::max();
    int64_t      expected[] = {INT_SCAN_SAMPLES, max};           // Insert the data as floats
    auto         samples    = {MAP_LIST(STR, INT_SCAN_SAMPLES)}; // Convert the data to strings
    stringstream iss;
    for (const auto &s : samples) {
        iss << s << " ";
    }
    iss << setprecision(20) << max;
    SETUP_SCANNER(iss);
    for (const auto &expect : expected) {
        EXPECT_VALUE_TOK(int64_t, INT, expect);
    }
    EXPECT_EOF();
}

TEST(ScannerTest, IntOverflow) {
    uint64_t     max = numeric_limits<int64_t>::max();
    stringstream iss;
    iss << max + 1;
    SETUP_SCANNER(iss);
    string msg = "int is out of range: " + to_string(max + 1);
    EXPECT_SYNTAX_ERROR(msg.c_str());
    auto tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, Float) {
#define FLOAT_SCAN_SAMPLES                                                                        \
    .1, .12, 1., 1.2, 1.23, 12., 12.3, 12.34, 1e2, .1e2, .12e3, 1.e2, 1.2e3, 1.23e4, 12e3, 12.e3, \
        12.3e4, 12.34e5, 1e+2, .1e+2, .12e+3, 1.e+2, 1.2e+3, 1.23e+4, 12e+3, 12.e+3, 12.3e+4,     \
        12.34e+5, 1e-2, .1e-2, .12e-3, 1.e-2, 1.2e-3, 1.23e-4, 12e-3, 12.e-3, 12.3e-4, 12.34e-5,  \
        1e23, .1e23, .12e34, 1.e23, 1.2e34, 1.23e45, 12e34, 12.e34, 12.3e45, 12.34e56, 1e+23,     \
        .1e+23, .12e+34, 1.e+23, 1.2e+34, 1.23e+45, 12e+34, 12.e+34, 12.3e+45, 12.34e+56, 1e-23,  \
        .1e-23, .12e-34, 1.e-23, 1.2e-34, 1.23e-45, 12e-34, 12.e-34, 12.3e-45, 12.34e-56
    double       max = numeric_limits<double>::max();
    double       min = numeric_limits<double>::min();
    stringstream iss;
    double       expected[] = {FLOAT_SCAN_SAMPLES, max, min};      // Insert the data as floats
    auto         samples    = {MAP_LIST(STR, FLOAT_SCAN_SAMPLES)}; // Convert the data to strings
    for (const auto &s : samples) {
        iss << s << " ";
    }
    iss << setprecision(20) << max << " " << min;
    SETUP_SCANNER(iss);
    for (const auto &expect : expected) {
        EXPECT_VALUE_TOK(double, FLOAT, expect);
    }
    EXPECT_EOF();
}

TEST(ScannerTest, NaN) {
    int           exampleCount = 8;
    istringstream iss(R"(nan Nan nAn NAn naN NaN nAN NAN)");
    SETUP_SCANNER(iss);
    for (int i = 0; i < exampleCount; i++) {
        auto tok = scanner.scan(drv);
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_FLOAT)
            << "expected a \"" << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_FLOAT)
            << "\" but got a \"" << tok.name() << "\"";
        auto got = tok.value.as<double>();
        EXPECT_TRUE(isnan(got));
    }
    EXPECT_EOF();
}

TEST(ScannerTest, Infinity) {
    int           exampleCount = 8;
    istringstream iss(R"(inf Inf iNf INf inF InF iNF INF)");
    SETUP_SCANNER(iss);
    for (int i = 0; i < exampleCount; i++) {
        auto tok = scanner.scan(drv);
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_FLOAT)
            << "expected a \"" << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_FLOAT)
            << "\" but got a \"" << tok.name() << "\"";
        auto got = tok.value.as<double>();
        EXPECT_TRUE(isinf(got));
    }
    EXPECT_EOF();
}

TEST(ScannerTest, Comment) {
    istringstream iss("// Comments should be ignored up to the line break\n"
                      "123 // Everything before a comment should be matched\n"
                      "// Comments at end of source should be ignored too");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(int64_t, INT, 123);
    EXPECT_EOF();
}

TEST(ScannerTest, BlockComment) {
    istringstream iss(R"(/* Block comments can span
                          * multiple lines.
                          */ )");
    SETUP_SCANNER(iss);
    auto tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockCommentsMustClose) {
    istringstream iss(R"(/* Block comments must be closed.
                                 If the file ends inside a block comment,
                                 it is an error.)");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("unexpected EOF in block comment");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, BlockCommentMustEndLine) {
    istringstream iss(R"(/* Block comments must be followed by line breaks */ foo)");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("block comment */ must be followed by a line break");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, IndependentBlockCommentsDontOutputNewlines) {
    istringstream iss("foo\n/* block comment on its own line */\n");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, BlockCommentOwnLine) {
    istringstream iss("foo\n/* block comment on its own line */\n");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, BlockCommentOwnLineEOF) {
    istringstream iss("foo\n/* block comment on its own line */");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, BlockCommentSameLine) {
    istringstream iss("foo /* block comment on same line */\n");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, BlockCommentSameLineEOF) {
    istringstream iss("foo /* block comment on same line */");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, NestedBlockComment) {
    istringstream iss(R"(/* Block comments can be
                              /* nested */ but every
                                 comment opening /* needs to be matched
                                 by an equivalent closing */ */)");
    SETUP_SCANNER(iss);
    auto tok = scanner.scan(drv);
    EXPECT_EQ(tok.type_get(), 0) << "Expected entire input to be consumed";
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NestedBlockCommentsMustClose) {
    istringstream iss(R"(/* Nested Block comments must also be closed.
                              /* Even if the inner block is closed, */
                                 failing to close the outer block by the end of
                                 the file is an error.)");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("unexpected EOF in block comment");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, MisplacedBlockCommentClose) {
    istringstream iss(R"(  */  )");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("unexpected */ outside of block comment");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, NoLeadingUnderscoresInIdent) {
    istringstream iss("_NoLeadingUnderscores");
    SETUP_SCANNER(iss);
    auto tok = scanner.scan(drv);
    EXPECT_NE(tok.type_get(), yy::Parser::symbol_kind_type::S_IDENT)
        << "Expected leading underscore to not be matched as part of an identifier";
}

TEST(ScannerTest, EmptyStringLiteral) {
    istringstream iss(R"("")");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, STRING, "");
    EXPECT_EOF();
}

TEST(ScannerTest, StringLiteral) {
    istringstream iss(R"("foo bar")");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, STRING, "foo bar");
    EXPECT_EOF();
}

TEST(ScannerTest, StringLiteralWithEscapedQuote) {
    istringstream iss(R"("foo\"bar")");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, STRING, R"(foo\"bar)");
    EXPECT_EOF();
}

TEST(ScannerTest, EscapedNewline) {
    istringstream iss("foo\\\nbar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, EscapedNewlineWithTrailingWhitespace) {
    istringstream iss("foo\\ \nbar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, NewlineInParensLeadingWitespace) {
    istringstream iss("foo(\n  bar\n)");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(LPAREN);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_TOK(RPAREN);
    EXPECT_EOF();
}

TEST(ScannerTest, NewlineInParens) {
    istringstream iss("foo(\nbar\n)");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(LPAREN);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_TOK(RPAREN);
    EXPECT_EOF();
}

TEST(ScannerTest, MixedTabWithSpaces) {
    istringstream iss("    \tfoo");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("inconsistent use of tabs and spaces in indentation");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, MixedSpaceWithTabs) {
    istringstream iss("\t foo");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("inconsistent use of tabs and spaces in indentation");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, InconsistentTabs) {
    istringstream iss("    foo\n\tbar");
    SETUP_SCANNER(iss);
    EXPECT_TOK(INDENT);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_SYNTAX_ERROR("inconsistent use of tabs and spaces in indentation");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, InconsistentSpaces) {
    istringstream iss("\tfoo\n    bar");
    SETUP_SCANNER(iss);
    EXPECT_TOK(INDENT);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_SYNTAX_ERROR("inconsistent use of tabs and spaces in indentation");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, EOFLineBreak) {
    istringstream iss("foo\n");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, EOFNoLineBreak) {
    istringstream iss("foo");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, EOFLineBreakTrailingWhitespace) {
    istringstream iss("foo\n    ");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, EOFNoLineBreakTrailingWhitespace) {
    istringstream iss("foo    ");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_EOF();
}

TEST(ScannerTest, TrailingSemicolon) {
    istringstream iss("foo;\nbar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(NEWLINE);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, TrailingSemicolonWhitespace) {
    istringstream iss("foo;    \nbar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(NEWLINE);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, TrailingSemicolons) {
    istringstream iss("foo;;\nbar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(NEWLINE);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, SemicolonSeparator) {
    istringstream iss("foo;bar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(SEMICOLON);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, SemicolonSeparatorWhitespace) {
    istringstream iss("foo;   bar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(SEMICOLON);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, SemicolonsSeparator) {
    istringstream iss("foo;;bar");
    SETUP_SCANNER(iss);
    EXPECT_VALUE_TOK(string, IDENT, "foo");
    EXPECT_TOK(SEMICOLON);
    EXPECT_VALUE_TOK(string, IDENT, "bar");
    EXPECT_EOF();
}

TEST(ScannerTest, InvalidEscapeCharacters) {
    string        chars = "abfv";
    string        input = "\a\b\f\v";
    istringstream iss(input);
    SETUP_SCANNER(iss);
    for (char c : chars) {
        string msg = "invalid character: '\\";
        msg += c;
        msg += "'";
        EXPECT_SYNTAX_ERROR(msg.c_str());
    }
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, HexEscapeCharacters) {
    istringstream iss("\x1B");
    SETUP_SCANNER(iss);
    EXPECT_SYNTAX_ERROR("invalid character: '0x1B'");
    EXPECT_EQ(oss.str(), "") << "Expected Flex to output no errors";
}

TEST(ScannerTest, SingleCloseParen) {
    istringstream iss(")");
    SETUP_SCANNER(iss);
    EXPECT_TOK(RPAREN);
    EXPECT_EOF();
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
