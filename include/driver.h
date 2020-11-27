#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <ast/statement.h>

namespace yy {
/// Forward declarations of classes
class Parser;
class Scanner;
class location;

class Driver {
public:
    Driver();
    ~Driver();

    int parse(std::istream &in = std::cin, std::ostream &out = std::cerr);
    int parse_file(const char *path);

    AST::Statement::Vec statements;

private:
    std::unique_ptr<Scanner>             scanner;
    std::unique_ptr<Parser>              parser;
    std::unique_ptr<yy::location>        location;
    std::vector<std::string>             lines;
    std::reference_wrapper<std::ostream> output;

    /// Allows Parser and Scanner to access private attributes
    /// of the Driver class
    friend class Parser;
    friend class Scanner;
};
} // namespace yy

#endif // DRIVER_H
