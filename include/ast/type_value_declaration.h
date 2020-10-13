#ifndef TYPE_VALUE_DECLARATION_H
#define TYPE_VALUE_DECLARATION_H

#include "value_declaration.h"
#include "type_declaration.h"

#include <memory>

namespace AST {

class TypeValueDeclaration final
    : public ValueDeclaration
    , public TypeDeclaration {
protected:
    void json(std::ostream &os) const override;

public:
    TypeValueDeclaration(
        const yy::location &               loc,
        const std::string &                variable,
        std::shared_ptr<TypeChecker::Type> type,
        std::unique_ptr<Expression>        value);
};

} // namespace AST

#endif // TYPE_DECLARATION_H
