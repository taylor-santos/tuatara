%{
#include <iostream>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>

#include "driver.h"
#include "parser.h"
%}

%option noyywrap nounput batch debug noinput nounistd never-interactive

ident   [a-zA-Z0-9][a-zA-Z0-9_]*
str     \"([^\\\"\n]|\\.)*\"
int     -?[0-9]+
float   (-?[0-9]+\.[0-9]*|-?[0-9]*\.[0-9]+)((e|E)-?[0-9]+)?|[0-9]+(e|E)-?[0-9]+|-?inf|-?nan

%{
    #define YY_USER_ACTION {            \
        loc.columns(yyleng);            \
        if (*yytext != '\n' &&          \
            *yytext != '\r')            \
            drv.lines.back() += yytext; \
    }
%}

%%

%{
    yy::location& loc = drv.location;
    loc.step();
%}

[ \t]+      loc.step();
[\n]        {
                loc.lines(1);
                loc.step();
                drv.lines.push_back("");
            }
\r

"//".*

"var"       return yy::parser::make_VAR(loc);
"="         return yy::parser::make_ASSIGN(loc);
";"         return yy::parser::make_SEMICOLON(loc);
":"         return yy::parser::make_COLON(loc);

{int}       {
                errno = 0;
                long n = strtol(yytext, NULL, 10);
                if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE)) {
                    throw yy::parser::syntax_error (
                        loc,
                        "integer is out of range: " + std::string(yytext)
                    );
                }
                return yy::parser::make_INT(n, loc);
            }
{float}     {
                errno = 0;
                double n = strtod(yytext, NULL);
                if (!(-HUGE_VAL <= n && n <= HUGE_VAL && errno != ERANGE)) {
                    throw yy::parser::syntax_error (
                        loc,
                        "double is out of range: " + std::string(yytext)
                    );
                }
                return yy::parser::make_FLOAT(n, loc);
            }
{ident}     return yy::parser::make_IDENT(yytext, loc);
{str}       return yy::parser::make_STRING(yytext, loc);

.           {
                std::stringstream ss;
                ss << "invalid character: ";
                if (std::isprint(*yytext)) {
                    ss << "'" << *yytext << "'";
                } else {
                    ss << "0x" << std::hex << (int)*yytext;
                }
                throw yy::parser::syntax_error(
                    loc,
                    ss.str()
                );
            }

<<EOF>>     return yy::parser::make_EOF(loc);

%%

void
Driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if (file.empty() || file == "-") {
        yyin = stdin;
    }
    else if (!(yyin = fopen (file.c_str (), "r"))) {
        std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void
Driver::scan_end() {
    fclose(yyin);
}