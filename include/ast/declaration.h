#ifndef DECLARATION_H
#define DECLARATION_H

#include "statement.h"

namespace AST {

class Declaration : public Statement {
protected: // Methods
    Declaration(const yy::location &loc, std::string variable);
    const std::string &getVariable() const;

private: // Fields
    std::string variable;
};

} // namespace AST

#endif // DECLARATION_H
