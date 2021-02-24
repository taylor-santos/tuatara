#ifndef AST_TYPE_VALUE_DECLARATION_H
#define AST_TYPE_VALUE_DECLARATION_H

#include "ast/type_declaration.h"
#include "ast/value_declaration.h"

namespace AST {

class TypeValueDeclaration final
    : public ValueDeclaration
    , public TypeDeclaration {
public: // Aliases
    using Ptr = std::unique_ptr<TypeValueDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    TypeValueDeclaration(
        const yy::location &   loc,
        const yy::location &   varLoc,
        const std::string &    variable,
        TypeChecker::Type::Ptr type,
        Expression::Ptr        value);
    [[nodiscard]] const std::string &getNodeName() const override;
    void walk(const std::function<void(const Node &)> &fn) const override;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_TYPE_DECLARATION_H
