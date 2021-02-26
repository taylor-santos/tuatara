#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 6326)
#    pragma warning(disable : 4389)
#endif

#include "common.h"
#include "driver.h"
#include "gtest/gtest.h"

using namespace std;

#define EXPECT_JSON(INPUT, TYPE, JSON)                                                           \
    do {                                                                                         \
        istringstream iss(INPUT);                                                                \
        ostringstream oss;                                                                       \
        yy::Driver    drv;                                                                       \
        EXPECT_EQ(drv.parse(iss, oss), 0);                                                       \
        EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";                        \
        ASSERT_EQ(drv.statements.size(), 1) << "Expected statements list to have one statement"; \
        EXPECT_NO_THROW({                                                                        \
            const auto &  node = dynamic_cast<AST::TYPE &>(*drv.statements[0]);                  \
            ostringstream ss;                                                                    \
            ss << node;                                                                          \
            EXPECT_EQ(ss.str(), JSON);                                                           \
        }) << "Expected AST node to be a \""                                                     \
           << #TYPE << "\" but got a \"" << drv.statements[0]->getNodeName() << "\"" << endl     \
           << *drv.statements[0];                                                                \
    } while (0)

TEST(ParserTest, EmptyFile) {
    istringstream iss("");
    ostringstream oss;
    yy::Driver    drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    EXPECT_TRUE(drv.statements.empty()) << "Expected statements list to be empty";
}

TEST(ParserTest, WhitespaceFile) {
    istringstream iss("    ");
    ostringstream oss;
    yy::Driver    drv;
    EXPECT_EQ(drv.parse(iss, oss), 0);
    EXPECT_EQ(oss.str(), "") << "Expected Bison to output no errors";
    EXPECT_TRUE(drv.statements.empty()) << "Expected statements list to be empty";
}

TEST(ParserTest, IntLiteral) {
    EXPECT_JSON("123", Int, R"({"node":"int","value":123})");
}

TEST(ParserTest, FloatLiteral) {
    EXPECT_JSON("123.456", Float, R"({"node":"float","value":123.456})");
}

TEST(ParserTest, StringLiteral) {
    EXPECT_JSON(R"("foobar")", String, R"({"node":"string","value":"foobar"})");
}

TEST(ParserTest, BoolLiteralTrue) {
    EXPECT_JSON("true\n", Bool, R"({"node":"bool","value":true})");
}

TEST(ParserTest, BoolLiteralFalse) {
    EXPECT_JSON("false\n", Bool, R"({"node":"bool","value":false})");
}

TEST(ParserTest, Variable) {
    EXPECT_JSON("abc", Variable, R"({"node":"variable","name":"abc"})");
}

TEST(ParserTest, Assignment) {
    EXPECT_JSON(
        "abc = 5",
        Call,
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"abc"},)"
        R"("field":"="},)"
        R"("arg":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ParserTest, ValueDeclaration) {
    EXPECT_JSON(
        "var abc = 123",
        ValueDeclaration,
        R"({"node":"value declaration",)"
        R"("variable":"abc",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ParserTest, TypeDeclaration) {
    EXPECT_JSON(
        "var abc: int",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"abc",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}})");
}

TEST(ParserTest, TypeValueDeclaration) {
    EXPECT_JSON(
        "var abc: int = 123",
        TypeValueDeclaration,
        R"({"node":"type value declaration",)"
        R"("variable":"abc",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"},)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ParserTest, IfBlock) {
    EXPECT_JSON(
        "if true then\n"
        "  abc\n"
        "  123",
        If,
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"abc"},{)"
        R"("node":"int",)"
        R"("value":123}]}})");
}

TEST(ParserTest, IfElseBlock) {
    EXPECT_JSON(
        "if true then\n"
        "  abc\n"
        "  123\n"
        "else\n"
        "  456\n",
        IfElse,
        R"({"node":"if else",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"abc"},{)"
        R"("node":"int",)"
        R"("value":123}]},)"
        R"("if false":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":456}]}})");
}

TEST(ParserTest, IfTernary) {
    EXPECT_JSON(
        "if true then 123",
        If,
        R"({"node":"if",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]}})");
}

TEST(ParserTest, IfElseTernary) {
    EXPECT_JSON(
        "if true then 123 else 456",
        IfElse,
        R"({"node":"if else",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("if true":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]},)"
        R"("if false":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":456}]}})");
}

TEST(ParserTest, WhileStatement) {
    EXPECT_JSON(
        "while true do\n"
        "  123",
        While,
        R"({"node":"while",)"
        R"("cond":{)"
        R"("node":"bool",)"
        R"("value":true},)"
        R"("block":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]}})");
}

TEST(ParserTest, MatchBlock) {
    EXPECT_JSON(
        "match foo\n"
        "  case x: int -> 5\n"
        "  case y= 0.5 -> 1.2\n"
        "  case z\n"
        "    123",
        Match,
        R"({"node":"match",)"
        R"("value":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("cases":[{)"
        R"("pattern":{)"
        R"("pattern":"named constraint",)"
        R"("name":"x",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}},)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":5}},{)"
        R"("pattern":{)"
        R"("pattern":"named constraint",)"
        R"("name":"y",)"
        R"("constraint":{)"
        R"("pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"float",)"
        R"("value":0.5}}},)"
        R"("body":{)"
        R"("node":"float",)"
        R"("value":1.2}},{)"
        R"("pattern":{)"
        R"("pattern":"named wildcard",)"
        R"("name":"z"},)"
        R"("body":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"int",)"
        R"("value":123}]}}]})");
}

TEST(ParserTest, FunctionCallNoArgs) {
    EXPECT_JSON(
        R"(foo())",
        Call,
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("arg":{)"
        R"("node":"unit"}})");
}

TEST(ParserTest, FunctionCallOneArg) {
    EXPECT_JSON(
        R"(foo(123))",
        Call,
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("arg":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ParserTest, FunctionCallTwoArgs) {
    EXPECT_JSON(
        R"(foo(123, 456))",
        Call,
        R"({"node":"function call",)"
        R"("function":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("arg":{)"
        R"("node":"tuple",)"
        R"("expressions":[{)"
        R"("node":"int",)"
        R"("value":123},)"
        R"({"node":"int",)"
        R"("value":456}]}})");
}

TEST(ParserTest, ArrayIndexOneArg) {
    EXPECT_JSON(
        R"(arr[123])",
        Index,
        R"({"node":"array index",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"arr"},)"
        R"("index":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ParserTest, ArrayIndexTwoArgs) {
    EXPECT_JSON(
        R"(arr[123, 456])",
        Index,
        R"({"node":"array index",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"arr"},)"
        R"("index":{)"
        R"("node":"tuple",)"
        R"("expressions":[{)"
        R"("node":"int",)"
        R"("value":123},)"
        R"({"node":"int",)"
        R"("value":456}]}})");
}

TEST(ParserTest, NullaryVoidFuncTypeDeclaration) {
    EXPECT_JSON(
        R"(var fn: (->))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"fn",)"
        R"("type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"unit"},)"
        R"("return type":{)"
        R"("kind":"unit"}}})");
}

TEST(ParserTest, VoidFuncTypeDeclaration) {
    EXPECT_JSON(
        R"(var fn: (int->))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"fn",)"
        R"("type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"int"},)"
        R"("return type":{)"
        R"("kind":"unit"}}})");
}

TEST(ParserTest, NullaryFuncTypeDeclaration) {
    EXPECT_JSON(
        R"(var fn: (->int))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"fn",)"
        R"("type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"unit"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}})");
}

TEST(ParserTest, FuncTypeDeclaration) {
    EXPECT_JSON(
        R"(var fn: (int->int))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"fn",)"
        R"("type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"int"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}})");
}

TEST(ParserTest, FuncTypeDeclarationPrecedence) {
    /* The type expression a -> b -> c should be equivalent to a -> (b -> c) */
    EXPECT_JSON(
        R"(var fn: (a -> b -> c))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"fn",)"
        R"("type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"a"},)"
        R"("return type":{)"
        R"("kind":"func",)"
        R"("arg":{)"
        R"("kind":"object",)"
        R"("class":"b"},)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"c"}}}})");
}

TEST(ParserTest, MultidimensionalArrays) {
    EXPECT_JSON(
        R"(var arr: int[][])",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"arr",)"
        R"("type":{)"
        R"("kind":"array",)"
        R"("type":{)"
        R"("kind":"array",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}}})");
}

TEST(ParserTest, ProductTypeDeclaration) {
    EXPECT_JSON(
        R"(var prod: (A, B, C))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"prod",)"
        R"("type":{)"
        R"("kind":"product",)"
        R"("types":[{)"
        R"("kind":"object",)"
        R"("class":"A"},)"
        R"({"kind":"object",)"
        R"("class":"B"},)"
        R"({"kind":"object",)"
        R"("class":"C"}]}})");
}

TEST(ParserTest, SumTypeDeclaration) {
    EXPECT_JSON(
        R"(var sum: (A | B | C))",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"sum",)"
        R"("type":{)"
        R"("kind":"sum",)"
        R"("types":[{)"
        R"("kind":"object",)"
        R"("class":"A"},)"
        R"({"kind":"object",)"
        R"("class":"B"},)"
        R"({"kind":"object",)"
        R"("class":"C"}]}})");
}

TEST(ParserTest, MaybeType) {
    EXPECT_JSON(
        "var foo: int?\n",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"foo",)"
        R"("type":{)"
        R"("kind":"maybe",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}})");
}

TEST(ParserTest, UnitType) {
    EXPECT_JSON(
        "var foo: ()\n",
        TypeDeclaration,
        R"({"node":"type declaration",)"
        R"("variable":"foo",)"
        R"("type":{)"
        R"("kind":"unit"}})");
}

TEST(ParserTest, UnitNode) {
    EXPECT_JSON(
        "var foo = ()\n",
        ValueDeclaration,
        R"({"node":"value declaration",)"
        R"("variable":"foo",)"
        R"("value":{)"
        R"("node":"unit"}})");
}

TEST(ParserTest, TupleExpression) {
    EXPECT_JSON(
        R"(1, 2.5, false, "foo")",
        Tuple,
        R"({"node":"tuple",)"
        R"("expressions":[{)"
        R"("node":"int",)"
        R"("value":1},)"
        R"({"node":"float",)"
        R"("value":2.5},)"
        R"({"node":"bool",)"
        R"("value":false},)"
        R"({"node":"string",)"
        R"("value":"foo"}]})");
}

TEST(ParserTest, InfixOperator) {
    EXPECT_JSON(
        R"(a #$% b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"#$%"},)"
        R"("ident":"b"})");
}

TEST(ParserTest, PostfixOperator) {
    EXPECT_JSON(
        R"(a#$%)",
        Field,
        R"({"node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"#$%"})");
}

TEST(ParserTest, PrefixOperator) {
    EXPECT_JSON(
        R"(#$%b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"#$%"},)"
        R"("ident":"b"})");
}

TEST(ParserTest, AssignOperator) {
    EXPECT_JSON(
        R"(a = b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"="},)"
        R"("ident":"b"})");
}

TEST(ParserTest, LessThanOperator) {
    EXPECT_JSON(
        R"(a < b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"<"},)"
        R"("ident":"b"})");
}

TEST(ParserTest, GreaterThanOperator) {
    EXPECT_JSON(
        R"(a > b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":">"},)"
        R"("ident":"b"})");
}

TEST(ParserTest, BitOrOperator) {
    EXPECT_JSON(
        R"(a | b)",
        IdentAccess,
        R"({"node":"ident access",)"
        R"("expr":{)"
        R"("node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"|"},)"
        R"("ident":"b"})");
}

TEST(ParserTest, QuestionOperator) {
    EXPECT_JSON(
        R"(a ?)",
        Field,
        R"({"node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"a"},)"
        R"("field":"?"})");
}

TEST(ParserTest, FieldAccess) {
    EXPECT_JSON(
        R"(foo.bar)",
        Field,
        R"({"node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("field":"bar"})");
}

TEST(ParserTest, OperatorAccess) {
    EXPECT_JSON(
        R"(foo.++)",
        Field,
        R"({"node":"field",)"
        R"("expr":{)"
        R"("node":"variable",)"
        R"("name":"foo"},)"
        R"("field":"++"})");
}

TEST(ParserTest, FuncImpl) {
    EXPECT_JSON(
        "func foo(x: int): int\n"
        "  x\n",
        FuncImpl,
        R"({"node":"function impl",)"
        R"("variable":"foo",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"x",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"int"},)"
        R"("body":{)"
        R"("node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"x"}]}})");
}

TEST(ParserTest, Lambda) {
    EXPECT_JSON(
        "func(x: int, _, =10): T -> 123\n",
        Lambda,
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"x",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}},{)"
        R"("pattern":"wildcard"},{)"
        R"("pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":10}}],)"
        R"("return type":{)"
        R"("kind":"object",)"
        R"("class":"T"},)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":123}})");
}

TEST(ParserTest, NamedWildcard) {
    EXPECT_JSON(
        "func(foo) -> 5\n",
        Lambda,
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"named wildcard",)"
        R"("name":"foo"}],)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ParserTest, LiteralPattern) {
    EXPECT_JSON(
        "func(5) -> 5\n",
        Lambda,
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"literal",)"
        R"("literal":{)"
        R"("node":"int",)"
        R"("value":5}}],)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ParserTest, ParenPattern) {
    EXPECT_JSON(
        "func((foo)) -> 5\n",
        Lambda,
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"named wildcard",)"
        R"("name":"foo"}],)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ParserTest, TuplePattern) {
    EXPECT_JSON(
        "func(a: int, (_, b:int, c=2)) -> 5\n",
        Lambda,
        R"({"node":"lambda",)"
        R"("args":[{)"
        R"("pattern":"named constraint",)"
        R"("name":"a",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}},{)"
        R"("pattern":"tuple",)"
        R"("patterns":[{)"
        R"("pattern":"wildcard"},{)"
        R"("pattern":"named constraint",)"
        R"("name":"b",)"
        R"("constraint":{)"
        R"("pattern":"type constraint",)"
        R"("type":{)"
        R"("kind":"object",)"
        R"("class":"int"}}},{)"
        R"("pattern":"named constraint",)"
        R"("name":"c",)"
        R"("constraint":{)"
        R"("pattern":"value constraint",)"
        R"("value":{)"
        R"("node":"int",)"
        R"("value":2}}}]}],)"
        R"("body":{)"
        R"("node":"int",)"
        R"("value":5}})");
}

TEST(ParserTest, SemicolonBetweenTwoStmts) {
    EXPECT_JSON(
        "foo;bar\n",
        Block,
        R"({"node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"foo"},{)"
        R"("node":"variable",)"
        R"("name":"bar"}]})");
}

TEST(ParserTest, SemicolonBetweenThreeStmts) {
    EXPECT_JSON(
        "foo;bar;baz\n",
        Block,
        R"({"node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"foo"},{)"
        R"("node":"variable",)"
        R"("name":"bar"},{)"
        R"("node":"variable",)"
        R"("name":"baz"}]})");
}

TEST(ParserTest, TrailingSemicolon) {
    EXPECT_JSON("foo;\n", Variable, R"({"node":"variable","name":"foo"})");
}

TEST(ParserTest, TrailingSemicolonTwoStmts) {
    EXPECT_JSON(
        "foo;bar;\n",
        Block,
        R"({"node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"foo"},{)"
        R"("node":"variable",)"
        R"("name":"bar"}]})");
}

TEST(ParserTest, ParenExpression) {
    EXPECT_JSON("(foo)\n", Variable, R"({"node":"variable","name":"foo"})");
}

TEST(ParserTest, BlockExpression) {
    EXPECT_JSON(
        "{\n"
        "  foo\n"
        "  bar\n"
        "}\n",
        Block,
        R"({"node":"block",)"
        R"("statements":[{)"
        R"("node":"variable",)"
        R"("name":"foo"},{)"
        R"("node":"variable",)"
        R"("name":"bar"}]})");
}

TEST(ParserTest, SyntaxError) {
    istringstream iss("func\n");
    ostringstream oss;
    yy::Driver    drv;
    EXPECT_NE(drv.parse(iss, oss), 0) << "Expected Bison to return an error code";
    EXPECT_EQ(
        oss.str(),
        "  <2:1>: syntax error: unexpected line break\n"
        "expected: \"->\", \":\", \"(\", \"=\", \"|\", \"?\", \"<\", \">\", \"identifier\", or "
        "\"operator\"\n")
        << "Expected Bison to output a syntax error message";
}

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
