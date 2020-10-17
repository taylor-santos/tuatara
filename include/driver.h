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

    void reset();

    std::vector<std::unique_ptr<AST::Statement>> statements;

private:
    std::unique_ptr<Scanner>      scanner;
    std::unique_ptr<Parser>       parser;
    std::unique_ptr<yy::location> location;
    std::vector<std::string>      lines;

    /// Allows Parser and Scanner to access private attributes
    /// of the Driver class
    friend class Parser;
    friend class Scanner;
};
} // namespace yy

#endif // DRIVER_H
