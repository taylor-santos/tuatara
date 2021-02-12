#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "ast/expression.h"

namespace AST {

class Declaration : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Declaration>;
    using Vec = std::vector<Ptr>;

protected: // Methods
    Declaration(const yy::location &loc, std::string variable);

    [[nodiscard]] const std::string &getVariable() const;

private: // Fields
    std::string variable_;
};

} // namespace AST

#endif // AST_DECLARATION_H
