#ifndef DRIVER_H
#define DRIVER_H

#include <functional>
#include <queue>
#include <stack>

#include "parser.tab.hh"

namespace AST {
class Expression;
} // namespace AST

namespace yy {
/// Forward declarations of classes
class Scanner;
class location;

class Driver {
public:
    Driver();
    ~Driver();

    int parse(std::istream &in = std::cin, std::ostream &out = std::cerr);
    int parseFile(const char *path);

    std::vector<std::unique_ptr<AST::Expression>> statements;
    std::vector<std::string>                      lines;

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
