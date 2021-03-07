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
    [[nodiscard]] const std::string &
    getVariable() const;
    std::shared_ptr<TypeChecker::Type>
    assignType(const std::shared_ptr<TypeChecker::Type> &type, TypeChecker::Context &ctx);

private: // Fields
    yy::location varLoc_;
    std::string  variable_;

private: // Methods
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) final;
    virtual std::shared_ptr<TypeChecker::Type>
    getDeclTypeImpl(TypeChecker::Context &ctx) = 0;
};

} // namespace AST

#endif // AST_DECLARATION_H
