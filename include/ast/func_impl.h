#ifndef AST_FUNC_IMPL_H
#define AST_FUNC_IMPL_H

#include "func_declaration.h"

namespace AST {

class FuncImpl final : public FuncDeclaration {
public: // Aliases
    using Ptr = std::unique_ptr<FuncImpl>;
    using Vec = std::vector<Ptr>;

public: // Methods
    FuncImpl(
        const yy::location &                                        loc,
        std::string                                                 variable,
        std::vector<std::pair<std::string, TypeChecker::Type::Ptr>> args,
        std::optional<TypeChecker::Type::Ptr>                       ret_type,
        Statement::Ptr                                              body);

private: // Fields
    Statement::Ptr body;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FUNC_IMPL_H
