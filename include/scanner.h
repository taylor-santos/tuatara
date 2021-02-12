#ifndef SCANNER_H
#define SCANNER_H

#include "parser.tab.hh"

#undef YY_DECL
#define YY_DECL yy::Parser::symbol_type yy::Scanner::scan(yy::Driver &driver)

#ifndef __FLEX_LEXER_H
#    if !defined(__clang__) && (defined(__GNUC__) || defined(__GNUG__))
#        pragma GCC diagnostic push
#        pragma GCC diagnostic ignored "-Wsuggest-override"
#    endif
#    include <FlexLexer.h>
#    if !defined(__clang__) && (defined(__GNUC__) || defined(__GNUG__))
#        pragma GCC diagnostic pop
#    endif
#endif

namespace yy {

class Scanner final : public yyFlexLexer {
public:
    Scanner();

    ~Scanner() override;

    virtual Parser::symbol_type scan(Driver &driver);

    [[maybe_unused]] void setDebug(bool b);
};

} // namespace yy

#endif // SCANNER_H
