%skeleton "lalr1.cc"
%require "3.2"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include "printer.h"

    using namespace std;

    class Driver;
}

// The parsing context.
%param { Driver &drv }

%initial-action { drv.lines.push_back(""); }

%locations

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.h"
    #include "location.hh"
}

%define api.token.prefix {TOK_}
%token
    EOF  0      "end of file"

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
    : %empty

%%

void
yy::parser::error(const location_type& l, const std::string& m) {
    Print::Error(m, l, drv.lines);
}