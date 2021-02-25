#ifndef AST_TYPE_VALUE_DECLARATION_H
#define AST_TYPE_VALUE_DECLARATION_H

#include "ast/type_declaration.h"
#include "ast/value_declaration.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class TypeValueDeclaration final
    : public ValueDeclaration
    , public TypeDeclaration {
public: // Methods
    TypeValueDeclaration(
        const yy::location &               loc,
        const yy::location &               varLoc,
        const std::string &                variable,
        std::unique_ptr<TypeChecker::Type> type,
        std::unique_ptr<Expression>        value);
    ~TypeValueDeclaration() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void walk(const std::function<void(const Node &)> &fn) const override;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_TYPE_DECLARATION_H
