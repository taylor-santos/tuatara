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
    TypeDeclaration(const yy::location &loc, std::string variable, TypeChecker::Type::Ptr type);
    [[nodiscard]] const std::string &getTypeName() const override;
    void                             walk(const Func &fn) const override;

protected: // Methods
    [[nodiscard]] const TypeChecker::Type &getType() const;
    void                                   json(std::ostream &os) const override;

private: // Fields
    TypeChecker::Type::Ptr type_;
};

} // namespace AST

#endif // AST_TYPE_DECLARATION_H
