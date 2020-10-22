#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <memory>

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class TypeDeclaration : virtual public Declaration {
protected: // Fields
    std::shared_ptr<TypeChecker::Type> type;

protected: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    TypeDeclaration(
        const yy::location &               loc,
        std::string                        variable,
        std::shared_ptr<TypeChecker::Type> type);
};

} // namespace AST

#endif // TYPE_DECLARATION_H
