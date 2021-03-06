#ifndef DRIVER_H
#define DRIVER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "parser.tab.hh"

namespace yy {
/// Forward declarations of classes
class Scanner;

class Driver {
public:
    Driver();
    ~Driver();

    int parse(std::istream &in = std::cin, std::ostream &out = std::cerr);
    int parseFile(const char *path);

    AST::Expression::Vec     statements;
    std::vector<std::string> lines;

private:
    std::unique_ptr<Scanner>             scanner;
    std::unique_ptr<Parser>              parser;
    std::unique_ptr<yy::location>        location;
    std::reference_wrapper<std::ostream> output;
    std::stack<int>                      indentStack;
    std::queue<Parser::symbol_type>      tokenQ;
    std::string                          filename;

    enum class IndentType { NOT_SET, SPACES, TABS } indentType = IndentType::NOT_SET;

    bool applyIndent(int indent);

    /// Allows Parser and Scanner to access private attributes
    /// of the Driver class
    friend class Parser;
    friend class Scanner;
};

} // namespace yy

#endif // DRIVER_H
