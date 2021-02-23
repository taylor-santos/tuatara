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
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getValue() const;
    TypeChecker::Type &             getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    Expression::Ptr value_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_VALUE_DECLARATION_H
