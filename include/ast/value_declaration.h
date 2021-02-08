#ifndef AST_VALUE_DECLARATION_H
#define AST_VALUE_DECLARATION_H

#include "declaration.h"
#include "expression.h"

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class ValueDeclaration : virtual public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<ValueDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    ValueDeclaration(const yy::location &loc, std::string variable, Expression::Ptr value);

protected: // Methods
    void                                 json(std::ostream &os) const override;
    [[nodiscard]] const Expression::Ptr &getValue() const;

private: // Fields
    Expression::Ptr value;
};

} // namespace AST

#endif // AST_VALUE_DECLARATION_H
