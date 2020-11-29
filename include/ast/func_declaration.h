#ifndef AST_FUNC_DECLARATION_H
#define AST_FUNC_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <optional>

namespace AST {

class FuncDeclaration : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncDeclaration(
        const yy::location &                                        loc,
        std::string                                                 variable,
        std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> args,
        std::optional<TypeChecker::Type::Ptr>                       ret_type);

protected: // Methods
    const std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> &getArgs() const;
    const std::optional<TypeChecker::Type::Ptr> &                      getRetType() const;

private: // Fields
    std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> args;
    std::optional<TypeChecker::Type::Ptr>                       ret_type;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FUNC_DECLARATION_H
