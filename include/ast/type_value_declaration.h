#ifndef TYPE_VALUE_DECLARATION_H
#define TYPE_VALUE_DECLARATION_H

#include "value_declaration.h"
#include "type_declaration.h"

#include <memory>

namespace AST {

class TypeValueDeclaration final
    : public ValueDeclaration
    , public TypeDeclaration {
public: // Methods
    TypeValueDeclaration(
        const yy::location &   loc,
        const std::string &    variable,
        TypeChecker::Type::Ptr type,
        Expression::Ptr        value);

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // TYPE_DECLARATION_H
