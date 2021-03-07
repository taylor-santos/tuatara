#ifndef AST_TYPE_DECLARATION_H
#define AST_TYPE_DECLARATION_H

#include "ast/declaration.h"

namespace TypeChecker {
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;
}

namespace AST {

// Virtually inherit from Declaration so TypeValueDeclaration can
// inherit from both ValueDeclaration and TypeDeclaration.
class TypeDeclaration : virtual public Declaration {
public: // Methods
    TypeDeclaration(
        const yy::location &               loc,
        const yy::location &               varLoc,
        std::string                        variable,
        std::shared_ptr<TypeChecker::Type> declType);
    ~TypeDeclaration() override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    void
    walk(const std::function<void(const Node &)> &fn) const override;

protected: // Methods
    // Constructor for virtual inheritance when Declaration has already been initialized
    TypeDeclaration(std::shared_ptr<TypeChecker::Type> declType);
    [[nodiscard]] const TypeChecker::Type &
    getDeclType() const;
    std::shared_ptr<TypeChecker::Type>
    getDeclTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::shared_ptr<TypeChecker::Type> declType_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_TYPE_DECLARATION_H
