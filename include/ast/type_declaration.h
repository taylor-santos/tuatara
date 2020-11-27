#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H

#include "declaration.h"
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

protected: // Methods
    const TypeChecker::Type::Ptr &getType() const;
    void                          json(std::ostream &os) const override;

private: // Fields
    TypeChecker::Type::Ptr type;
};

} // namespace AST

#endif // TYPE_DECLARATION_H
