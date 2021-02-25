#ifndef AST_FIELD_H
#define AST_FIELD_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Type;
class Context;
} // namespace TypeChecker

namespace AST {

class Field final : public LValue {
public: // Methods
    Field(
        const yy::location &        loc,
        std::unique_ptr<Expression> expr,
        const yy::location &        fieldLoc,
        std::string                 field);
    ~Field() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> expr_;
    yy::location                fieldLoc_;
    std::string                 field_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FIELD_H
