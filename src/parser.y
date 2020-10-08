%skeleton "lalr1.cc"
%require "3.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define api.value.automove
%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires {
    #include "printer.h"
    #include "ast/int.h"
    #include "ast/float.h"
    #include "ast/string.h"
    #include "ast/variable.h"
    #include "ast/assignment.h"
    #include "ast/type_declaration.h"
    #include "ast/value_declaration.h"

    #include <memory>

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

%type<unique_ptr<AST::Expression>>
    literal
    primary_expression
    assignment
    expression
%type<unique_ptr<AST::Statement>>
    declaration
    stmt
%type<vector<unique_ptr<AST::Statement>>>
    stmts
    opt_stmts

%start file

%%

file
    : opt_stmts {
        drv.statements = $1;
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
        $$ = make_unique<AST::TypeDeclaration>(@$, $2);
    }

expression
    : primary_expression
    | assignment

assignment
    : "identifier" "=" primary_expression {
        $$ = make_unique<AST::Assignment>(@$, $1, $3);
    }

primary_expression
    : literal
    | "identifier" {
        $$ = make_unique<AST::Variable>(@$, $1);
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

type
    : "identifier"

%%

void
yy::parser::error(const location_type& l, const std::string& m) {
    Print::Error(m, l, drv.lines);
}