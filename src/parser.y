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

    using std::make_unique, std::make_shared, std::make_pair;
%}

%define api.token.prefix {TOK_}
%token
    EOF  0      "end of file"
    VAR         "var"
    FUNC        "func"
    ARROW       "->"
    RETURN      "return"
    IF          "if"
    WHILE       "while"
    ELSE        "else"
    ASSIGN      "="
    SEMICOLON   ";"
    COLON       ":"
    LPAREN      "("
    RPAREN      ")"
    LSQUARE     "["
    RSQUARE     "]"
    LCURLY      "{"
    RCURLY      "}"
    COMMA       ","
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
    func_decl
    stmt
    one_line_stmt
    if_stmt
    while_stmt
    return_stmt
    stmt_block
%type<StatementVec>
    stmts
    opt_stmts
%type<ExpressionVec>
    expr_list
    opt_expr_list
%type<TypePtr>
    type
    array_type
    base_type
%type<TypeVec>
    types
%type<OptType>
    opt_type
    opt_array_type
%type<NamedTypeVec>
    opt_types_decl
    types_decl
%type<NamedType>
    type_decl

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
    : one_line_stmt
    | if_stmt
    | while_stmt
    | return_stmt

one_line_stmt
    : declaration
    | expression ";" {
        $$ = $1;
    }
    | stmt_block

stmt_block
    : "{" opt_stmts "}" {
        $$ = make_unique<AST::Block>(@$, $2);
    }

declaration
    : "var" "identifier" "=" expression ";" {
        $$ = make_unique<AST::ValueDeclaration>(@$, $2, $4);
    }
    | "var" "identifier" ":" type ";" {
        $$ = make_unique<AST::TypeDeclaration>(@$, $2, $4);
    }
    | "var" "identifier" ":" type "=" expression ";" {
        $$ = make_unique<AST::TypeValueDeclaration>(@$, $2, $4, $6);
    }
    | func_decl

expression
    : primary_expression
    | assignment

if_stmt
    : "if" expression one_line_stmt {
        $$ = make_unique<AST::If>(@$, $2, $3);
    }
    | "if" expression one_line_stmt "else" stmt {
        $$ = make_unique<AST::If>(@$, $2, $3, $5);
    }

while_stmt
    : "while" expression one_line_stmt {
        $$ = make_unique<AST::While>(@$, $2, $3);
    }

return_stmt
    : "return" ";" {
        $$ = make_unique<AST::Return>(@$);
    }
    | "return" expression ";" {
        $$ = make_unique<AST::Return>(@$, $2);
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
    | primary_expression "(" opt_expr_list ")" {
        $$ = make_unique<AST::Call>(@$, $1, $3);
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
    : %empty {}
    | type {
        $$ = $1;
    }

type
    : array_type
    | opt_type "->" opt_array_type {
        $$ = make_shared<TypeChecker::Func>(@$, $1, $3);
    }

opt_array_type
    : %empty {}
    | array_type {
        $$ = $1;
    }

array_type
    : base_type
    | array_type "[" "]" {
        $$ = make_shared<TypeChecker::Array>(@$, $1);
    }

base_type
    : "identifier" {
        $$ = make_shared<TypeChecker::Object>(@$, $1);
    }
    | "(" types ")" {
        $$ = make_shared<TypeChecker::Tuple>(@$, $2);
    }

types
    : type {
        $$.push_back($1);
    }
    | types "," type {
        $$ = $1;
        $$.push_back($3);
    }

opt_expr_list
    : %empty {}
    | expr_list

expr_list
    : expression {
        $$.push_back($1);
    }
    | expr_list "," expression {
        $$ = $1;
        $$.push_back($3);
    }

func_decl
    : "func" "identifier" "(" opt_types_decl ")" "->" opt_type stmt_block {
        $$ = make_unique<AST::FuncDeclaration>(@$, $2, $4, $7, $8);
    }

opt_types_decl
    : %empty {}
    | types_decl {
        $$ = $1;
    }

types_decl
    : type_decl {
        $$.push_back($1);
    }
    | types_decl "," type_decl {
        $$ = $1;
        $$.push_back($3);
    }

type_decl
    : "identifier" ":" type {
        $$ = make_pair($1, $3);
    }

%%

namespace yy {

void
Parser::error(const location_type& l, const std::string& m) {
    Print::Error(driver.output, m, l, driver.lines);
}

}
