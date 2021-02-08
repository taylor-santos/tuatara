%{

#include "parser.tab.hh"
#include "scanner.h"
#include "driver.h"

#define STEP()                                                          \
    do {                                                                \
        driver.location->step ();                                       \
    } while (0)

#define COL(Col)                                                        \
    driver.location->columns (Col)

#define LINE(Line)                                                      \
    do {                                                                \
        driver.location->lines (Line);                                  \
        driver.lines.insert(driver.lines.end(), Line, "");              \
        STEP();                                                         \
    } while (0)

#define YY_USER_ACTION                                                  \
    {                                                                   \
        COL(yyleng);                                                    \
        if (*yytext != '\n' && *yytext != '\r') {                       \
            driver.lines.back() += yytext;                              \
        }                                                               \
    }


typedef yy::Parser::token token;
typedef yy::Parser::token_type token_type;

#define LOC                                                             \
    (*driver.location)

#define return_token(TOK)                                               \
    return yy::Parser::make_##TOK(*driver.location)
#define return_value_token(TOK, value)                                  \
    return yy::Parser::make_##TOK(value, *driver.location)

#define yyterminate() return_token(EOF)

%}

%option debug
%option c++
%option noyywrap
%option never-interactive
%option yylineno
%option nounput
%option batch
%option prefix="yy"
%option stack

%x COMMENT

ident   [a-zA-Z0-9][a-zA-Z0-9_]*
str     \"([^\\\"\n]|\\.)*\"
digit   [0-9]
exp     [eE][+-]?{digit}+
blank   [ \t]+
nan     (?i:nan)
inf     (?i:inf)
op      [+\-*/\\=%<>&|!@#$^~]+

%{

#include <climits>
#include <cmath>
#include <sstream>

#define LEX_FLOAT()                                                     \
    do {                                                                \
        try {                                                           \
            return_value_token(FLOAT, std::stod(yytext));               \
        } catch (const std::out_of_range &e) {                          \
            throw yy::Parser::syntax_error(                             \
                LOC,                                                    \
                std::string{"float is out of range: "} + yytext);       \
        } catch (const std::invalid_argument &e) {                      \
            throw yy::Parser::syntax_error(                             \
                LOC,                                                    \
                std::string{"invalid float: "} + yytext);               \
        }                                                               \
    } while(0)

#define LEX_INT()                                                       \
    do {                                                                \
        try {                                                           \
            return_value_token(INT, std::stoll(yytext));                \
        } catch (const std::out_of_range &e) {                          \
            throw yy::Parser::syntax_error(                             \
                LOC,                                                    \
                std::string{"int is out of range: "} + yytext);         \
        } catch (const std::invalid_argument &e) {                      \
            throw yy::Parser::syntax_error(                             \
                LOC,                                                    \
                std::string{"invalid int: "} + yytext);                 \
        }                                                               \
    } while(0)

%}

%%

%{
    STEP();
%}

{blank}     STEP();

"\n"+       LINE(yyleng);
"\r"+

"//".*      // Ignore comments

"/*"        yy_push_state(COMMENT);
"*/"        throw yy::Parser::syntax_error(LOC, "unexpected */ outside of block comment");
<COMMENT>{
    "/*"        yy_push_state(COMMENT);
    "*/"        yy_pop_state();
    "\n"+       LINE(yyleng);
    "\r"+
    .
    <<EOF>>     throw yy::Parser::syntax_error(LOC, "unexpected EOF in block comment");
}

"var"       return_token(VAR);
"func"      return_token(FUNC);
"->"        return_token(ARROW);
"return"    return_token(RETURN);
"class"     return_token(CLASS);
"impl"      return_token(IMPL);
"operator"  return_token(OPERATOR);
"new"       return_token(NEW);
"if"        return_token(IF);
"while"     return_token(WHILE);
"else"      return_token(ELSE);
"then"      return_token(THEN);
"do"        return_token(DO);
"true"      return_value_token(BOOL, true);
"false"     return_value_token(BOOL, false);
"="         return_token(ASSIGN);
";"         return_token(SEMICOLON);
":"         return_token(COLON);
"("         return_token(LPAREN);
")"         return_token(RPAREN);
"["         return_token(LSQUARE);
"]"         return_token(RSQUARE);
"{"         return_token(LCURLY);
"}"         return_token(RCURLY);
"<"         return_token(LANGLE);
">"         return_token(RANGLE);
","         return_token(COMMA);
"&"         return_token(AND);
"|"         return_token(OR);
{op}        return_value_token(OPERATION, yytext);

{digit}+                    { LEX_INT(); }
{digit}+{exp}               { LEX_FLOAT(); }
{digit}*"."{digit}+({exp})? { LEX_FLOAT(); }
{digit}+"."{digit}*({exp})? { LEX_FLOAT(); }
{nan}                       { return_value_token(FLOAT, NAN); }
{inf}                       { return_value_token(FLOAT, INFINITY); }

{ident}     return_value_token(IDENT, yytext);
{str}       {
                std::string str = yytext;
                //assert(str[0] == '"');
                //assert(str[str.size()-1] == '"');
                str = str.substr(1, str.size()-2);
                return_value_token(STRING, str);
            }

.           {
                std::stringstream ss;
                ss << "invalid character: ";
                if (std::isprint(*yytext)) {
                    ss << "'" << *yytext << "'";
                } else {
                    ss << "0x" << std::hex << (int)*yytext;
                }
                throw yy::Parser::syntax_error(
                    LOC,
                    ss.str()
                );
            }

<<EOF>>     return_token(EOF);

%%

namespace yy {

Scanner::Scanner()
    : yyFlexLexer() {}

Scanner::~Scanner() {}

void
Scanner::setDebug(bool b) {
    yy_flex_debug = b;
}

}

#ifdef yylex
#undef yylex
#endif

int
yyFlexLexer::yylex() {
    std::cerr << "Do not call yyFlexLexer::yylex()!" << std::endl;
    return 0;
}
