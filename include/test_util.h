#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include "type/type_context.h"

#include "driver.h"
#include "gtest/gtest.h"
#include "scanner.h"

#define EXPECT_JSON(INPUT, TYPE, JSON)                                                           \
    do {                                                                                         \
        std::istringstream iss(INPUT);                                                           \
        std::ostringstream oss;                                                                  \
        yy::Driver         drv;                                                                  \
        EXPECT_EQ(drv.parse(iss, oss), 0);                                                       \
        EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";                        \
        ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement"; \
        EXPECT_NO_THROW({                                                                        \
            const auto &       node = dynamic_cast<AST::TYPE &>(*drv.statements[0]);             \
            std::ostringstream ss;                                                               \
            ss << node;                                                                          \
            EXPECT_EQ(ss.str(), JSON);                                                           \
        }) << "Expected AST node to be a \""                                                     \
           << #TYPE << "\" but got a \"" << drv.statements[0]->getNodeName() << "\"" << endl     \
           << *drv.statements[0];                                                                \
    } while (0)

#define STR(x) #x

#define EXPECT_NO_ASSERT(statement)   \
    EXPECT_EXIT(                      \
        {                             \
            statement;                \
            exit(0);                  \
        },                            \
        ::testing::ExitedWithCode(0), \
        ".*")

#define SETUP_SCANNER(INPUT_STREAM)              \
    yy::Scanner        scanner;                  \
    std::ostringstream oss;                      \
    scanner.switch_streams((INPUT_STREAM), oss); \
    yy::Driver drv;

#define EXPECT_VALUE_TOK(TYPE, TOKEN, VALUE)                                                       \
    do {                                                                                           \
        auto tok = scanner.scan(drv);                                                              \
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_##TOKEN)                         \
            << "expected value " << (VALUE) << " to be a \""                                       \
            << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_##TOKEN)                    \
            << "\" but got a \"" << tok.name() << "\"";                                            \
        auto got = tok.value.as<TYPE>();                                                           \
        EXPECT_EQ(got, VALUE) << "expected value \"" << (VALUE) << "\" but got \"" << got << "\""; \
    } while (0)

#define EXPECT_TOK(TOKEN)                                                                          \
    do {                                                                                           \
        auto tok = scanner.scan(drv);                                                              \
        ASSERT_EQ(tok.type_get(), yy::Parser::symbol_kind_type::S_##TOKEN)                         \
            << "expected a \"" << yy::Parser::symbol_name(yy::Parser::symbol_kind_type::S_##TOKEN) \
            << "\" but got a \"" << tok.name() << "\"";                                            \
    } while (0)

#define EXPECT_EOF()         \
    EXPECT_NO_THROW({        \
        EXPECT_TOK(NEWLINE); \
        EXPECT_TOK(YYEOF);   \
    })

#define EXPECT_SYNTAX_ERROR(ERROR)                            \
    do {                                                      \
        EXPECT_THROW(                                         \
            {                                                 \
                try {                                         \
                    auto tok = scanner.scan(drv);             \
                } catch (const yy::Parser::syntax_error &e) { \
                    EXPECT_STREQ(e.what(), (ERROR));          \
                    throw;                                    \
                }                                             \
            },                                                \
            yy::Parser::syntax_error);                        \
    } while (0)

#define EXPECT_TYPE(INPUT_STREAM, SYMBOL, TYPE)                                                   \
    do {                                                                                          \
        SETUP_SCANNER((INPUT_STREAM));                                                            \
        EXPECT_EQ(drv.parse(iss, oss), 0);                                                        \
        EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";                         \
        TypeChecker::Context ctx;                                                                 \
        for (auto &stmt : drv.statements) {                                                       \
            stmt->getType(ctx);                                                                   \
        }                                                                                         \
        auto symbol = ctx.getSymbol((SYMBOL));                                                    \
        ASSERT_TRUE(symbol) << "Expected \"" << (SYMBOL) << "\" to be added to the symbol table"; \
        auto              new_type = TypeChecker::Type::verify((TYPE), ctx);                      \
        std::stringstream ss;                                                                     \
        ss << "Expected type \"";                                                                 \
        new_type->pretty(ss);                                                                     \
        ss << "\" but got type \"";                                                               \
        (*symbol)->pretty(ss);                                                                    \
        ss << "\"";                                                                               \
        EXPECT_TRUE((*symbol)->isEqual(*new_type, ctx)) << ss.str();                              \
    } while (0)
#endif
