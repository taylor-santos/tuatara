#ifndef AST_TYPE_DECLARATION_H
#define AST_TYPE_DECLARATION_H

#include "ast/declaration.h"

#include "type/type.h"

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class TypeDeclaration : virtual public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<TypeDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    TypeDeclaration(const yy::location &loc, std::string variable, TypeChecker::Type::Ptr declType);
    [[nodiscard]] const std::string &getNodeName() const override;
    void walk(const std::function<void(const Node &)> &fn) const override;

protected: // Methods
    [[nodiscard]] const TypeChecker::Type &getDeclType() const;
    TypeChecker::Type &                    getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    TypeChecker::Type::Ptr declType_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_TYPE_DECLARATION_H
