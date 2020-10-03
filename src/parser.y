%skeleton "lalr1.cc"
%require "3.2"
%defines

%define api.token.constructor
%define api.value.type variant
//TODO: uncomment this line when the parser builds AST nodes:
//%define api.value.automove
%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires {
    #include "printer.h"

    using namespace std;

    class Driver;
}

// The parsing context.
%param { Driver &drv }

%initial-action { drv.lines.push_back(""); }

%locations

%code {
    #include "driver.h"
    #include "location.hh"
}

%define api.token.prefix {TOK_}
%token
    EOF  0      "end of file"
    VAR         "var"
    ASSIGN      "="
    SEMICOLON   ";"
    COLON       ":"

%token<string>
    IDENT   "identifier"
    STRING  "string literal"
%token<int>
    INT     "int literal"
%token<double>
    FLOAT   "float literal"

%start file

%%

file
    : opt_stmts

opt_stmts
    : %empty
    | stmts

stmts
    : stmt
    | stmts stmt

stmt
    : declaration ";"
    | expression ";"

declaration
    : "var" assignment
    | "var" "identifier" ":" type

expression
    : primary_expression
    | assignment

assignment
    : "identifier" opt_type_decl "=" primary_expression

primary_expression
    : "identifier"
    | literal

literal
    : "int literal"
    | "float literal"
    | "string literal"

opt_type_decl
    : %empty
    | type_decl

type_decl
    : ":" type

type
    : "identifier"

%%

void
yy::parser::error(const location_type& l, const std::string& m) {
    Print::Error(m, l, drv.lines);
}