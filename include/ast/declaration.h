#ifndef AST_DECLARATION_H
#define AST_DECLARATION_H

#include "ast/expression.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker

namespace AST {

class Declaration : public Expression {
protected: // Methods
    Declaration(const yy::location &loc, const yy::location &varLoc, std::string variable);
    ~Declaration() override;
    [[nodiscard]] const std::string &getVariable() const;
    /// Add a type to the symbol table if the variable name is new, or update it if not.
    /// Throws a TypeException if the new type is incompatible with an existing type.
    /// \param ctx The type checker context.
    /// \param type The new type to be added.
    /// \param init True if the type is guaranteed to represent an initialized value
    void assignType(TypeChecker::Context &ctx, TypeChecker::Type &type, bool init) const;

private: // Fields
    yy::location varLoc_;
    std::string  variable_;
};

} // namespace AST

#endif // AST_DECLARATION_H
