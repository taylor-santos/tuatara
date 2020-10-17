#ifndef SCANNER_H
#define SCANNER_H

#include "parser.tab.hh"

#ifndef YY_DECL
#    define YY_DECL yy::Parser::symbol_type yy::Scanner::yylex(yy::Driver &driver)
#endif

#ifndef __FLEX_LEXER_H
#    define yyFlexLexer yyFlexLexer
#    include <FlexLexer.h>
#    undef yyFlexLexer
#endif

namespace yy {

class Scanner : public yyFlexLexer {
public:
    Scanner();

    virtual ~Scanner();

    virtual Parser::symbol_type yylex(Driver &driver);

    void set_debug(bool b);
};

} // namespace yy

#endif // SCANNER_H
