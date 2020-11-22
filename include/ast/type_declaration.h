#ifndef TYPE_DECLARATION_H
#define TYPE_DECLARATION_H

#include "declaration.h"
#include "type/type.h"

#include <memory>

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class TypeDeclaration : virtual public Declaration {
public: // Methods
    TypeDeclaration(
        const yy::location &               loc,
        std::string                        variable,
        std::shared_ptr<TypeChecker::Type> type);

protected: // Methods
    const std::shared_ptr<TypeChecker::Type> &getType() const;
    void                                      json(std::ostream &os) const override;

private: // Fields
    std::shared_ptr<TypeChecker::Type> type;
};

} // namespace AST

#endif // TYPE_DECLARATION_H
