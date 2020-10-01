%{
#include <iostream>
#include <string>
#include <sstream>

#include "driver.h"
#include "parser.h"
%}

%option noyywrap nounput batch debug noinput nounistd never-interactive

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