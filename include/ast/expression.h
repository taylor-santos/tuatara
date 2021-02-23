#ifndef AST_EXPRESSION_H
#define AST_EXPRESSION_H

#include "type/type.h"
#include "type/type_context.h"

#include "ast.h"

namespace AST {

class Expression : public Node {
public: // Aliases
    using Ptr = std::unique_ptr<Expression>;
    using Vec = std::vector<Ptr>;

public: // Methods
    TypeChecker::Type &getType(TypeChecker::Context &ctx);

protected: // Methods
    explicit Expression(const yy::location &loc);
    virtual TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) = 0;

private: // Fields
    TypeChecker::Type *myType_;
};

} // namespace AST

#endif // AST_EXPRESSION_H
