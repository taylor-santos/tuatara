#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "statement.h"

namespace AST {

class Declaration : public Statement {
public: // Aliases
    using Ptr = std::unique_ptr<Declaration>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    Declaration(const yy::location &loc, std::string variable);

    [[nodiscard]] const std::string &getVariable() const;

private: // Fields
    std::string variable;
};

} // namespace AST

#endif // AST_DECLARATION_H
