#ifndef DRIVER_H
#define DRIVER_H

#include "parser.h"
#include "ast/statement.h"

#define YY_DECL yy::parser::symbol_type yylex(Driver &drv)
YY_DECL;

class Driver {
public:
    Driver();

    // Run the parser on file f.  Return 0 on success.
    int parse(const std::string &f);
    // The name of the file being parsed.
    std::string file;
    // The source code lines after invoking parse().
    std::vector<std::string> lines;
    // The generated AST after invoking parse().
    std::vector<std::unique_ptr<AST::Statement>> statements;
    // Whether to generate parser debug traces.
    bool trace_parsing;

    // Handling the scanner.
    void scan_begin();
    void scan_end();
    // Whether to generate scanner debug traces.
    bool trace_scanning;
    // The token's location used by the scanner.
    yy::location location;
};
#endif // DRIVER_H