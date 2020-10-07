#ifndef DECLARATION_H
#define DECLARATION_H

#include "statement.h"

namespace TuataraAST {

class Declaration : public Statement {
protected:
    std::string variable;

protected:
    Declaration(const yy::location &loc, std::string variable);
};

} // namespace TuataraAST

#endif // DECLARATION_H
