%{
    #include "parser.tab.hh"
    #include "scanner.h"

    #define yylex driver.scanner->yylex
%}

%code requires {
    #include <iostream>
    #include "driver.h"
    #include "location.hh"
    #include "common.h"
}

%code provides {
    namespace yy {

        inline void
        yyerror(const char* msg) {
            std::cerr << msg << std::endl;
        }

    }
}

%require "3.3"
%language "C++"
%locations
%defines
%token-table
%parse-param {Driver &driver}
%lex-param {Driver &driver}

%define api.namespace {yy}
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant
%define api.value.automove
%define parse.assert
%define parse.trace
%define parse.error verbose

%{
    #include "printer.h"

    using std::make_unique, std::make_shared;
%}

%define api.token.prefix {TOK_}
%token
    EOF  0      "end of file"
    VAR         "var"
    IF          "if"
    ELSE        "else"
    TRUE        "true"
    FALSE       "false"
    ASSIGN      "="
    SEMICOLON   ";"
    COLON       ":"
    LPAREN      "("
    RPAREN      ")"
    LSQUARE     "["
    RSQUARE     "]"
    LCURLY      "{"
    RCURLY      "}"
    PLUS        "+"
    PLUSEQ      "+="
    MINUSEQ     "-="
    TIMESEQ     "*="
    DIVIDESEQ   "/="
    EQ          "=="
    GTEQ        ">="
    LTEQ        "<="
    GT          ">"
    LT          "<"
    TIMES       "*"
    MINUS       "-"
    DIVIDES     "/"
    MODULO      "%"
    AMPERSAND   "&"
    PIPE        "|"
    LSHIFT      "<<"
    RSHIFT      ">>"
    AND         "&&"
    OR          "||"

%token<std::string>
    IDENT   "identifier"
    STRING  "string literal"
%token<int64_t>
    INT     "int literal"
%token<double>
    FLOAT   "float literal"
%token<bool>
    BOOL    "bool literal"

%type<ExpressionPtr>
    literal
    primary_expression
    assignment
    expression
%type<LValuePtr>
    lvalue
%type<StatementPtr>
    declaration
    stmt
    one_line_stmt
%type<StatementVec>
    stmts
    opt_stmts
%type<TypePtr>
    type

%start file

%%

file
    : opt_stmts {
        driver.statements = $1;
    }

opt_stmts
    : %empty {}
    | stmts

stmts
    : stmt {
        $$.push_back($1);
    }
    | stmts stmt {
        $$ = $1;
        $$.push_back($2);
    }

stmt
    : one_line_stmt {
        $$ = $1;
    }
    | if_stmt {
        //TODO: If statement AST node
    }
    | "{" opt_stmts "}" {
        $$ = make_unique<AST::Block>(@$, $2);
    }

one_line_stmt
    : declaration ";" {
        $$ = $1;
    }
    | expression ";" {
        $$ = $1;
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

expression
    : primary_expression
    | assignment

if_stmt
    : "if" expression "{" opt_stmts "}" opt_else {
        //TODO: If statement AST node
        throw Parser::syntax_error(@1, "If statements not implemented");
    }
    | "if" expression one_line_stmt opt_else {
        //TODO: If statement AST node
        throw Parser::syntax_error(@1, "If statements not implemented");
    }

opt_else
    : %empty {
        //TODO: If statement AST node
    }
    | "else" stmt {
        //TODO: If statement AST node
    }

assignment
    : lvalue "=" primary_expression {
        $$ = make_unique<AST::Assignment>(@$, $1, $3);
    }

primary_expression
    : literal
    | lvalue {
        $$ = $1;
    }

lvalue
    : "identifier" {
        $$ = make_unique<AST::Variable>(@$, $1);
    }
    | "identifier" ":" type {
        $$ = make_unique<AST::TypedVariable>(@$, $1, $3);
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

type
    : "identifier" {
        $$ = make_unique<TypeChecker::Object>(@$, $1);
    }

%%

namespace yy {

void
Parser::error(const location_type& l, const std::string& m) {
    Print::Error(driver.output, m, l, driver.lines);
}

}
