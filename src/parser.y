%require "3.6.1"
%language "C++"
%locations
%defines
%parse-param {class Driver &driver}
%lex-param {class Driver &driver}

%define api.namespace {yy}
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define api.value.automove
%define parse.assert
%define parse.trace
%define parse.error custom
%define parse.lac full
%define api.token.prefix {TOK_}

%{
// This block is inserted at the top of parser.tab.cc

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4005)
#    pragma warning(disable : 4127)
#    pragma warning(disable : 4244)
#endif

#include <sstream>

#include "driver.h"
#include "scanner.h"
#include "printer.h"

#define yylex driver.scanner->scan
#define STRINGIFY_(X) #X
#define STRINGIFY(X) STRINGIFY_(X)
#define LOC_STR __FILE__ ":" STRINGIFY(__LINE__)

using std::make_unique, std::make_pair;

namespace yy {

void
Parser::error(const location_type& l, const std::string& m) {
    Print::error(driver.output, m, l, driver.lines);
}

void
Parser::report_syntax_error(yy::Parser::context const &ctx) const {
    const auto &lookahead = ctx.lookahead();
    auto location = ctx.location();
    int num = ctx.expected_tokens(nullptr, 100);
    std::stringstream ss;
    ss << "syntax error: unexpected " << lookahead.name();
    Print::error(driver.output, ss.str(), location, driver.lines);
    std::unique_ptr<symbol_kind_type[]> expected(new symbol_kind_type[num]);
    ctx.expected_tokens(expected.get(), num);
    std::string sep = "expected: ";
    std::string end = "";
    for (int i = 0; i < num - 1; i++) {
        driver.output.get() << sep << "\"" << symbol_name(expected[i]) << "\"";
        sep = ", ";
        end = "or ";
    }
    driver.output.get() << sep << end << "\"" << symbol_name(expected[num-1]) << "\"" << std::endl;
}

} // namespace yy

%}

%code requires {
// This block is inserted at the top of parser.tab.hh

#include <iostream>

#include "location.hh"
#include "common.h"

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4065)
#endif

} // %code requires

%code provides {
// This block is inserted at the bottom of parser.tab.hh

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

} // %code provides

%token
    EOF  0      "end of file"
    NEWLINE     "line break"
    INDENT      "indent"
    OUTDENT     "outdent"
    VAR         "var"
    FUNC        "func"
    ARROW       "->"
    IF          "if"
    THEN        "then"
    ELSE        "else"
    WHILE       "while"
    DO          "do"
    MATCH       "match"
    CASE        "case"
    SEMICOLON   ";"
    ASSIGN      "="
    COLON       ":"
    LPAREN      "("
    RPAREN      ")"
    LSQUARE     "["
    RSQUARE     "]"
    LBRACE      "{"
    RBRACE      "}"
    LANGLE      "<"
    RANGLE      ">"
    COMMA       ","
    OR          "|"
    QUESTION    "?"
    WILDCARD    "_"

%token<std::string>
    IDENT       "identifier"
    STRING      "string literal"
    OPERATION   "operator"
%token<int64_t>
    INT     "int literal"
%token<double>
    FLOAT   "float literal"
%token<bool>
    BOOL    "bool literal"

%type<AST::Expression::Ptr>
    primary_expression
    expression_line
    expression
    operator_expression
    declaration
    func_impl
    while_expression
    if_expression
    match_expression
    ternary
    lambda
    value_pattern
%type<AST::Expression::Vec>
    opt_expressions
    expressions
    tuple_expression
    multi_expression
    multi_semi_expression
%type<std::optional<AST::Expression::Ptr>>
    opt_expression
%type<AST::Literal::Ptr>
    literal
%type<AST::Block::Ptr>
    block_expression
%type<AST::LValue::Ptr>
    lvalue
%type<AST::Match::Case>
    case
%type<std::vector<AST::Match::Case>>
    cases
%type<TypeChecker::Type::Ptr>
    type
    sum_type
    post_type
    base_type
    func_type
    type_pattern
    opt_type
    opt_sum_type
%type<TypeChecker::Type::Vec>
    sum_type_list
    product_type_list
%type<std::optional<TypeChecker::Type::Ptr>>
    opt_ret_type
%type<Pattern::Pattern::Ptr>
    pattern
%type<Pattern::Pattern::Vec>
    opt_patterns
    patterns
    pattern_list
%type<Pattern::Constraint::Ptr>
    constraint_pattern
%type<std::string>
    operator

%start file

%%

file
    : opt_expressions {
        driver.statements = $1;
    }

opt_expressions
    : %empty {}
    | expressions

expression_line
    : expression "line break" {
        @$ = @1;
        $$ = $1;
    }
    | if_expression
    | while_expression
    | match_expression

expressions
    : expression_line {
        $$.push_back($1);
    }
    | expressions expression_line {
        $$ = $1;
        $$.push_back($2);
    }

expression
    : operator_expression
    | declaration
    | lambda
    | ternary
    | multi_expression {
        $$ = make_unique<AST::Block>(@$, $1);
    }
    | tuple_expression {
        $$ = make_unique<AST::Tuple>(@$, $1);
    }

semicolons
    : ";"
    | semicolons ";"

multi_semi_expression
    : operator_expression semicolons {
        $$.push_back($1);
    }
    | multi_semi_expression operator_expression semicolons {
        $$ = $1;
        $$.push_back($2);
    }

multi_expression
    : multi_semi_expression
    | multi_semi_expression operator_expression {
        $$ = $1;
        $$.push_back($2);
    }

declaration
    : "var" "identifier" "=" expression {
        $$ = make_unique<AST::ValueDeclaration>(@$, $2, $4);
    }
    | "var" "identifier" ":" type {
        $$ = make_unique<AST::TypeDeclaration>(@$, $2, $4);
    }
    | "var" "identifier" ":" type "=" expression {
        $$ = make_unique<AST::TypeValueDeclaration>(@$, $2, $4, $6);
    }
    | func_impl {
        $$ = $1;
    }

tuple_expression
    : operator_expression "," operator_expression {
        $$.push_back($1);
        $$.push_back($3);
    }
    | tuple_expression "," operator_expression {
        $$ = $1;
        $$.push_back($3);
    }

operator_expression
    : primary_expression
    | primary_expression operator primary_expression {
        $$ = make_unique<AST::InfixOperator>(@$, $2, $1, $3);
    }
    | primary_expression operator {
        $$ = make_unique<AST::PostfixOperator>(@$, $2, $1);
    }
    | operator primary_expression {
        $$ = make_unique<AST::PrefixOperator>(@$, $1, $2);
    }

operator
    : "operator"
    | "=" {
        $$ = "=";
    }
    | "<" {
        $$ = "<";
    }
    | ">" {
        $$ = ">";
    }
    | "|" {
        $$ = "|";
    }
    | "?" {
        $$ = "?";
    }

block_expression
    : "line break" "indent" expressions "outdent" "line break" {
        @$ = @3;
        $$ = make_unique<AST::Block>(@$, $3);
    }

if_expression
    : "if" expression "then" block_expression {
        $$ = make_unique<AST::If>(@$, $2, $4);
    }
    | "if" expression "then" block_expression "else" block_expression {
        $$ = make_unique<AST::IfElse>(@$, $2, $4, $6);
    }

ternary
    : "if" expression "then" operator_expression {
        AST::Expression::Vec stmts;
        stmts.emplace_back($4);
        $$ = make_unique<AST::If>(@$, $2, make_unique<AST::Block>(@4, move(stmts)));
    }
    | "if" expression "then" operator_expression "else" operator_expression {
        AST::Expression::Vec trueStmts, falseStmts;
        trueStmts.emplace_back($4);
        falseStmts.emplace_back($6);
        $$ = make_unique<AST::IfElse>(
            @$,
            $2,
            make_unique<AST::Block>(@4, move(trueStmts)),
            make_unique<AST::Block>(@6, move(falseStmts))
        );
    }

while_expression
    : "while" expression "do" block_expression {
        $$ = make_unique<AST::While>(@$, $2, $4);
    }

match_expression
    : "match" expression "line break" "indent" cases "outdent" "line break" {
        $$ = make_unique<AST::Match>(@$, $2, $5);
    }

cases
    : case {
        $$.push_back($1);
    }
    | cases case {
        $$ = $1;
        $$.push_back($2);
    }

case
    : "case" pattern "->" expression "line break" {
        $$ = make_pair($2, $4);
    }
    | "case" pattern block_expression {
        $$ = make_pair($2, $3);
    }

primary_expression
    : literal {
        $$ = $1;
    }
    | lvalue {
        $$ = $1;
    }
    | "(" expression ")" {
        $$ = $2;
    }
    | "{" "line break" "indent" expressions "outdent" "line break" "}" {
        $$ = make_unique<AST::Block>(@$, $4);
    }
    | "(" ")" {
        $$ = make_unique<AST::Unit>(@$);
    }

lvalue
    : "identifier" {
        $$ = make_unique<AST::Variable>(@$, $1);
    }
    | primary_expression "(" opt_expression ")" {
        $$ = make_unique<AST::Call>(@$, $1, $3);
    }
    | primary_expression "[" expression "]" {
        $$ = make_unique<AST::Index>(@$, $1, $3);
    }

opt_expression
    : %empty {}
    | expression {
        $$ = move($1);
    }

literal
    : "int literal" {
        $$ = make_unique<AST::Int>(@$, $1);
    }
    | "float literal" {
        $$ = make_unique<AST::Float>(@$, $1);
    }
    | "string literal" {
        $$ = make_unique<AST::String>(@$, $1);
    }
    | "bool literal" {
        $$ = make_unique<AST::Bool>(@$, $1);
    }

opt_type
    : %empty {
        $$ = make_unique<TypeChecker::Unit>(@$);
    }
    | type {
        $$ = $1;
    }

type
    : sum_type

sum_type
    : post_type {
        $$ = $1;
    }
    | sum_type_list {
        $$ = make_unique<TypeChecker::Sum>(@$, $1);
    }

func_type
    : opt_sum_type "->" opt_type {
        $$ = make_unique<TypeChecker::Func>(@$, $1, $3);
    }
    | opt_sum_type "->" func_type {
        $$ = make_unique<TypeChecker::Func>(@$, $1, $3);
    }

opt_sum_type
    : %empty {
        $$ = make_unique<TypeChecker::Unit>(@$);
    }
    | sum_type {
        $$ = $1;
    }

sum_type_list
    : post_type "|" post_type {
        $$.push_back($1);
        $$.push_back($3);
    }
    | sum_type_list "|" post_type {
        $$ = $1;
        $$.push_back($3);
    }

post_type
    : base_type
    | post_type "[" "]" {
        $$ = make_unique<TypeChecker::Array>(@$, $1);
    }
    | post_type "?" {
        $$ = make_unique<TypeChecker::Maybe>(@$, $1);
    }

base_type
    : "identifier" {
        $$ = make_unique<TypeChecker::Object>(@$, $1);
    }
    | "(" product_type_list ")" {
        $$ = make_unique<TypeChecker::Product>(@$, $2);
    }
    | "(" func_type ")" {
        $$ = $2;
    }
    | "(" type ")" {
        $$ = $2;
    }
    | "(" ")" {
        $$ = make_unique<TypeChecker::Unit>(@$);
    }

product_type_list
    : type "," type {
        $$.push_back($1);
        $$.push_back($3);
    }
    | product_type_list "," type {
        $$ = $1;
        $$.push_back($3);
    }

func_impl
    : "func" "identifier" opt_patterns opt_ret_type "line break" "indent" expressions "outdent" {
        @$ = yy::location{@1.begin, @7.end};
        auto block = make_unique<AST::Block>(@7, $7);
        $$ = make_unique<AST::FuncImpl>(@$, $2, $3, move(block), $4);
    }

lambda
    : "func" opt_patterns opt_ret_type "->" expression {
        $$ = make_unique<AST::Lambda>(@$, $2, $3, $5);
    }

opt_ret_type
    : %empty {}
    | ":" type {
        $$ = $2;
    }

opt_patterns
    : %empty {}
    | "(" ")" {}
    | "(" patterns ")" {
        $$ = $2;
    }

patterns
    : pattern {
        $$.push_back($1);
    }
    | pattern_list {
        $$ = $1;
    }

pattern_list
    : pattern "," pattern {
        $$.push_back($1);
        $$.push_back($3);
    }
    | pattern_list "," pattern {
        $$ = $1;
        $$.push_back($3);
    }

pattern
    : "identifier" {
        $$ = make_unique<Pattern::NamedWildcard>(@$, $1);
    }
    | literal {
        $$ = make_unique<Pattern::Literal>(@$, $1);
    }
    | "_" {
        $$ = make_unique<Pattern::Wildcard>(@$);
    }
    | "(" pattern ")" {
        $$ = $2;
    }
    | "(" pattern_list ")" {
        $$ = make_unique<Pattern::Tuple>(@$, $2);
    }
    | constraint_pattern {
        $$ = $1;
    }
    | "identifier" constraint_pattern {
        $$ = make_unique<Pattern::NamedConstraint>(@$, $1, $2);
    }

constraint_pattern
    : value_pattern {
        $$ = make_unique<Pattern::ValueConstraint>(@$, $1);
    }
    | type_pattern {
        $$ = make_unique<Pattern::TypeConstraint>(@$, $1);
    }

type_pattern
    : ":" type {
        $$ = $2;
    }

value_pattern
    : "=" operator_expression {
        $$ = $2;
    }

%%

// This block is inserted at the bottom of parser.tab.cc

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
