#ifndef PATTERN_VALUE_CONSTRAINT_H
#define PATTERN_VALUE_CONSTRAINT_H

#include "pattern/constraint.h"

namespace AST {
class Expression;
} // namespace AST
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class ValueConstraint final : public Constraint {
public: // Methods
    ValueConstraint(const yy::location &loc, std::unique_ptr<AST::Expression> value);
    ~ValueConstraint() override;

    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::unique_ptr<AST::Expression> value_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_VALUE_CONSTRAINT_H
