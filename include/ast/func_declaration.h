#ifndef AST_FUNC_DECLARATION_H
#define AST_FUNC_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <optional>
#include <functional>

namespace AST {

class FuncDeclaration : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncDeclaration(
        const yy::location &                  loc,
        std::string                           variable,
        std::vector<TypeChecker::Type::Named> args,
        TypeChecker::Type::Ptr                retType);

    FuncDeclaration(
        const yy::location &                  loc,
        std::string                           variable,
        std::vector<TypeChecker::Type::Named> args);

protected: // Methods
    [[nodiscard]] const std::vector<TypeChecker::Type::Named> &                  getArgs() const;
    [[nodiscard]] std::optional<std::reference_wrapper<const TypeChecker::Type>> getRetType() const;

private: // Fields
    std::vector<TypeChecker::Type::Named> args;
    TypeChecker::Type::Ptr                retType;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FUNC_DECLARATION_H
