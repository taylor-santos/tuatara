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

    using std::make_unique, std::make_pair;
%}

%define api.token.prefix {TOK_}
%token
    EOF  0      "end of file"
    VAR         "var"
    FUNC        "func"
    ARROW       "->"
    RETURN      "return"
    CLASS       "class"
    IMPL        "impl"
    NEW         "new"
    OPERATOR    "operator"
    IF          "if"
    WHILE       "while"
    ELSE        "else"
    THEN        "then"
    DO          "do"
    ASSIGN      "="
    SEMICOLON   ";"
    COLON       ":"
    LPAREN      "("
    RPAREN      ")"
    LSQUARE     "["
    RSQUARE     "]"
    LCURLY      "{"
    RCURLY      "}"
    LANGLE      "<"
    RANGLE      ">"
    COMMA       ","
    AND         "&"
    OR          "|"

%token<std::string>
    IDENT       "identifier"
    STRING      "string literal"
    OPERATION   "operation"
%token<int64_t>
    INT     "int literal"
%token<double>
    FLOAT   "float literal"
%token<bool>
    BOOL    "bool literal"

%type<AST::Expression::Ptr>
    literal
    primary_expression
    expression
    operator_expression
%type<AST::Expression::Vec>
    tuple_expression
%type<std::optional<AST::Expression::Ptr>>
    opt_expression
%type<AST::LValue::Ptr>
    lvalue
%type<AST::Statement::Ptr>
    declaration
    func_impl
    class_decl
    stmt
    one_line_stmt
    if_stmt
    while_stmt
    return_stmt
    stmt_block
%type<AST::Statement::Vec>
    stmts
    opt_stmts
%type<TypeChecker::Type::Ptr>
    type
    sum_type
    product_type
    array_type
    base_type
%type<TypeChecker::Type::Vec>
    sum_type_list
    product_type_list
%type<OptType>
    opt_type
    opt_sum_type
%type<NamedTypeVec>
    opt_types_decl
    types_decl
%type<NamedType>
    type_decl
%type<std::string>
    operation
    super
%type<std::vector<std::string>>
    opt_supers
    supers
%type<AST::ClassDeclaration::Members>
    opt_member_decls
    member_decls
%type<AST::ClassDeclaration::Field>
    field_decl
%type<AST::FuncDeclaration::Ptr>
    func_decl
%type<AST::ClassDeclaration::Operator>
    operator_decl
%type<AST::ClassDeclaration::Constructor>
    ctor_decl

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
    | func_decl ";" {
        $$ = $1;
    }
    | func_impl
    | class_decl

opt_expression
    : %empty {}
    | expression {
        $$ = $1;
    }

expression
    : operator_expression
    | tuple_expression {
        $$ = make_unique<AST::Tuple>(@$, $1);
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
    | primary_expression operation primary_expression {
        $$ = make_unique<AST::Operator>(@$, $2, $1, $3);
    }

operation
    : "operation"
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

if_stmt
    : "if" expression "then" one_line_stmt {
        $$ = make_unique<AST::If>(@$, $2, $4);
    }
    | "if" expression "then" one_line_stmt "else" stmt {
        $$ = make_unique<AST::If>(@$, $2, $4, $6);
    }

while_stmt
    : "while" expression "do" one_line_stmt {
        $$ = make_unique<AST::While>(@$, $2, $4);
    }

return_stmt
    : "return" ";" {
        $$ = make_unique<AST::Return>(@$);
    }
    | "return" expression ";" {
        $$ = make_unique<AST::Return>(@$, $2);
    }

primary_expression
    : literal
    | lvalue {
        $$ = $1;
    }
    | "(" expression ")" {
        $$ = $2;
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
    : sum_type
    | opt_sum_type "->" opt_type {
        $$ = make_unique<TypeChecker::Func>(@$, $1, $3);
    }

opt_sum_type
    : %empty {}
    | sum_type {
        $$ = $1;
    }

sum_type
    : product_type {
        $$ = $1;
    }
    | sum_type_list {
        $$ = make_unique<TypeChecker::Sum>(@$, $1);
    }

sum_type_list
    : product_type "|" product_type {
        $$.push_back($1);
        $$.push_back($3);
    }
    | sum_type_list "|" product_type {
        $$ = $1;
        $$.push_back($3);
    }

product_type
    : array_type {
        $$ = $1;
    }
    | product_type_list {
        $$ = make_unique<TypeChecker::Product>(@$, $1);
    }

product_type_list
    : array_type "&" array_type {
        $$.push_back($1);
        $$.push_back($3);
    }
    | product_type_list "&" array_type {
        $$ = $1;
        $$.push_back($3);
    }

array_type
    : base_type
    | array_type "[" "]" {
        $$ = make_unique<TypeChecker::Array>(@$, $1);
    }

base_type
    : "identifier" {
        $$ = make_unique<TypeChecker::Object>(@$, $1);
    }
    | "(" type ")" {
        $$ = $2;
    }

func_decl
    : "func" "identifier" "(" opt_types_decl ")" "->" type {
        $$ = make_unique<AST::FuncDeclaration>(@$, $2, $4, $7);
    }
    | "func" "identifier" "(" opt_types_decl ")" {
        $$ = make_unique<AST::FuncDeclaration>(@$, $2, $4);
    }

func_impl
    : "func" "identifier" "(" opt_types_decl ")" "->" type stmt_block {
        $$ = make_unique<AST::FuncImpl>(@$, $2, $4, $7, $8);
    }
    | "func" "identifier" "(" opt_types_decl ")" stmt_block {
        $$ = make_unique<AST::FuncImpl>(@$, $2, $4, $6);
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

class_decl
    : "class" "identifier" opt_supers "{" opt_member_decls "}" {
        $$ = make_unique<AST::ClassDeclaration>(@$, $2, $3, $5);
    }

opt_supers
    : %empty {}
    | ":" supers {
        $$ = $2;
    }

supers
    : super {
        $$.push_back($1);
    }
    | supers "," super {
        $$ = $1;
        $$.push_back($3);
    }

super
    : "identifier"

opt_member_decls
    : %empty {}
    | member_decls

member_decls
    : field_decl ";" {
        $$.fields.push_back($1);
    }
    | func_decl ";" {
        $$.methods.push_back($1);
    }
    | operator_decl ";" {
        $$.operators.push_back($1);
    }
    | ctor_decl ";" {
        $$.ctors.push_back($1);
    }
    | member_decls field_decl ";" {
        $$ = $1;
        $$.fields.push_back($2);
    }
    | member_decls func_decl ";" {
        $$ = $1;
        $$.methods.push_back($2);
    }
    | member_decls operator_decl ";" {
        $$ = $1;
        $$.operators.push_back($2);
    }
    | member_decls ctor_decl ";" {
        $$ = $1;
        $$.ctors.push_back($2);
    }

field_decl
    : "identifier" ":" type {
        $$ = AST::ClassDeclaration::Field{$1, $3};
    }

operator_decl
    : "operator" operation "(" type_decl ")" "->" opt_type {
        $$ = AST::ClassDeclaration::Operator{$2, $4, $7};
    }

ctor_decl
    : "new" "(" opt_types_decl ")" {
        $$ = AST::ClassDeclaration::Constructor{$3};
    }

%%

namespace yy {

void
Parser::error(const location_type& l, const std::string& m) {
    Print::Error(driver.output, m, l, driver.lines);
}

}
