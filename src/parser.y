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
#include "ast/bool.h"
#include "ast/call.h"
#include "ast/field.h"
#include "ast/float.h"
#include "ast/func_impl.h"
#include "ast/ident_access.h"
#include "ast/if.h"
#include "ast/if_else.h"
#include "ast/index.h"
#include "ast/int.h"
#include "ast/lambda.h"
#include "ast/match.h"
#include "ast/string.h"
#include "ast/tuple.h"
#include "ast/type_declaration.h"
#include "ast/type_value_declaration.h"
#include "ast/unit.h"
#include "ast/variable.h"
#include "ast/while.h"
#include "driver.h"
#include "pattern/literal.h"
#include "pattern/named_constraint.h"
#include "pattern/named_wildcard.h"
#include "pattern/tuple.h"
#include "pattern/type_constraint.h"
#include "pattern/value_constraint.h"
#include "printer.h"
#include "scanner.h"
#include "type/array.h"
#include "type/func.h"
#include "type/maybe.h"
#include "type/object.h"
#include "type/product.h"
#include "type/sum.h"
#include "type/unit.h"

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

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4065)
#endif

#include <optional>

#include "ast/block.h"
#include "ast/literal.h"
#include "type/object.h"
#include "type/type.h"
#include "pattern/constraint.h"

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
    COLON       ":"
    LPAREN      "("
    RPAREN      ")"
    LSQUARE     "["
    RSQUARE     "]"
    LBRACE      "{"
    RBRACE      "}"
    COMMA       ","
    WILDCARD    "_"
// Valid ids that are used in other grammar rules:
    ASSIGN      "="
    OR          "|"
    QUESTION    "?"
    LANGLE      "<"
    RANGLE      ">"

%token<std::string>
    IDENT       "identifier"
    OPERATOR    "operator"
    STRING      "string literal"
%token<int64_t>
    INT     "int literal"
%token<double>
    FLOAT   "float literal"
%token<bool>
    BOOL    "bool literal"

%type<std::unique_ptr<AST::Expression>>
    primary_expression
    expression_line
    expression
    access_expression
    ident_expression
    declaration
    func_impl
    while_expression
    if_expression
    match_expression
    ternary
    lambda
    value_pattern
%type<std::vector<std::unique_ptr<AST::Expression>>>
    opt_expressions
    file_expressions
    expressions
    tuple_expression
    multi_expression
    multi_semi_expression
%type<std::unique_ptr<AST::Literal>>
    literal
%type<std::unique_ptr<AST::Block>>
    block_expression
%type<std::pair<std::unique_ptr<Pattern::Pattern>,std::unique_ptr<AST::Expression>>>
    case
%type<std::vector<std::pair<std::unique_ptr<Pattern::Pattern>,std::unique_ptr<AST::Expression>>>>
    cases
%type<std::unique_ptr<TypeChecker::Type>>
    type
    sum_type
    post_type
    base_type
    func_type
    type_pattern
    opt_type
    opt_sum_type
%type<std::vector<std::unique_ptr<TypeChecker::Type>>>
    sum_type_list
    product_type_list
%type<std::optional<std::unique_ptr<TypeChecker::Type>>>
    opt_ret_type
%type<std::unique_ptr<Pattern::Pattern>>
    pattern
%type<std::vector<std::unique_ptr<Pattern::Pattern>>>
    opt_patterns
    patterns
    pattern_list
%type<std::unique_ptr<Pattern::Constraint>>
    constraint_pattern
%type<std::string>
    ident
    operator
    func_name
%start file

%%

file
    : opt_expressions {
        driver.statements = $1;
    }

opt_expressions
    : %empty {}
    | file_expressions

file_expressions
    : expression_line {
        $$.push_back($1);
        //TODO
        //std::cout << *$$.back() << std::endl;
    }
    | file_expressions expression_line {
        $$ = $1;
        $$.push_back($2);
        //TODO
        //std::cout << *$$.back() << std::endl;
    }

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
    : ident_expression
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
    : ident_expression semicolons {
        $$.push_back($1);
    }
    | multi_semi_expression ident_expression semicolons {
        $$ = $1;
        $$.push_back($2);
    }

multi_expression
    : multi_semi_expression
    | multi_semi_expression ident_expression {
        $$ = $1;
        $$.push_back($2);
    }

declaration
    : "var" ident "=" expression {
        $$ = make_unique<AST::ValueDeclaration>(@$, @2, $2, $4);
    }
    | "var" ident ":" type {
        $$ = make_unique<AST::TypeDeclaration>(@$, @2, $2, $4);
    }
    | "var" ident ":" type "=" expression {
        $$ = make_unique<AST::TypeValueDeclaration>(@$, @2, $2, $4, $6);
    }
    | func_impl {
        $$ = $1;
    }

tuple_expression
    : ident_expression "," ident_expression {
        $$.push_back($1);
        $$.push_back($3);
    }
    | tuple_expression "," ident_expression {
        $$ = $1;
        $$.push_back($3);
    }

ident
    : "identifier"

operator
    : "operator"
    | "=" {
        $$ = "=";
    }
    | "|" {
        $$ = "|";
    }
    | "?" {
        $$ = "?";
    }
    | "<" {
        $$ = "<";
    }
    | ">" {
        $$ = ">";
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
    : "if" expression "then" ident_expression {
        std::vector<std::unique_ptr<AST::Expression>> stmts;
        stmts.emplace_back($4);
        $$ = make_unique<AST::If>(@$, $2, make_unique<AST::Block>(@4, move(stmts)));
    }
    | "if" expression "then" ident_expression "else" ident_expression {
        std::vector<std::unique_ptr<AST::Expression>> trueStmts, falseStmts;
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

ident_expression
    : access_expression
    | ident {
        $$ = make_unique<AST::Variable>(@$, $1);
    }
    | operator {
        $$ = make_unique<AST::Variable>(@$, $1);
    }

access_expression
    : primary_expression
    | ident_expression "." ident {
        $$ = make_unique<AST::Field>(@$, $1, @3, $3);
    }
    | ident_expression "." operator {
        $$ = make_unique<AST::Field>(@$, $1, @3, $3);
    }
    | ident_expression ident {
        $$ = make_unique<AST::IdentAccess>(@$, $1, @2, $2);
    }
    | ident_expression operator {
        $$ = make_unique<AST::Field>(@$, $1, @2, $2);
    }
    | ident_expression primary_expression {
        $$ = make_unique<AST::Call>(@$, $1, $2);
    }
    | ident_expression "[" expression "]" {
        $$ = make_unique<AST::Index>(@$, $1, $3);
    }

primary_expression
    : literal {
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
    : ident {
        $$ = make_unique<TypeChecker::Object>(@$, $1);
    }
    | "(" product_type_list ")" {
        $$ = make_unique<TypeChecker::ManagedProduct>(@$, $2);
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

func_name
    : ident
    | operator

func_impl
    : "func" func_name opt_patterns opt_ret_type "line break" "indent" expressions "outdent" {
        @$ = yy::location{@1.begin, @7.end};
        auto block = make_unique<AST::Block>(@7, $7);
        $$ = make_unique<AST::FuncImpl>(@$, @2, $2, $3, move(block), $4);
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
    : ident {
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
    | ident constraint_pattern {
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
    : "=" ident_expression {
        $$ = $2;
    }

%%

// This block is inserted at the bottom of parser.tab.cc

#ifdef _MSC_VER
#    pragma warning(pop)
#endif
