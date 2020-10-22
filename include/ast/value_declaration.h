#ifndef VALUE_DECLARATION_H
#define VALUE_DECLARATION_H

#include "declaration.h"
#include "expression.h"

#include <memory>

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class ValueDeclaration : virtual public Declaration {
protected: // Fields
    std::unique_ptr<Expression> value;

protected: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    ValueDeclaration(
        const yy::location &        loc,
        std::string                 variable,
        std::unique_ptr<Expression> value);
};

} // namespace AST

#endif // VALUE_DECLARATION_H
