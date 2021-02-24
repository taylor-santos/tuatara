#ifndef AST_FIELD_H
#define AST_FIELD_H

#include <optional>

#include "ast/lvalue.h"

namespace AST {

class Field final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Field>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Field(
        const yy::location &loc,
        Expression::Ptr     expr,
        const yy::location &fieldLoc,
        std::string         field);
    ~Field() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr expr_;
    yy::location    fieldLoc_;
    std::string     field_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FIELD_H
