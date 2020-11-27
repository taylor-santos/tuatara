#ifndef FUNC_DECLARATION_H
#define FUNC_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <optional>

namespace AST {

class FuncDeclaration final : public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncDeclaration(
        const yy::location &                                        loc,
        std::string                                                 variable,
        std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> args,
        std::optional<TypeChecker::Type::Ptr>                       ret_type,
        Statement::Ptr                                              body);

private: // Fields
    std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> args;
    std::optional<TypeChecker::Type::Ptr>                       ret_type;
    Statement::Ptr                                              body;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // FUNC_DECLARATION_H
