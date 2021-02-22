#ifndef AST_VALUE_DECLARATION_H
#define AST_VALUE_DECLARATION_H

#include "ast/declaration.h"
#include "ast/expression.h"

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class ValueDeclaration : virtual public Declaration {
public: // Aliases
    using Ptr = std::unique_ptr<ValueDeclaration>;
    using Vec = std::vector<Ptr>;

public: // Methods
    ValueDeclaration(const yy::location &loc, std::string variable, Expression::Ptr value);
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

protected: // Methods
    void                            json(std::ostream &os) const override;
    [[nodiscard]] const Expression &getValue() const;

private: // Fields
    Expression::Ptr value_;
};

} // namespace AST

#endif // AST_VALUE_DECLARATION_H
