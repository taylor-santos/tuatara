#ifndef PATTERN_VALUE_CONSTRAINT_H
#define PATTERN_VALUE_CONSTRAINT_H

#include "ast/expression.h"

#include "pattern/constraint.h"

namespace Pattern {

class ValueConstraint final : public Constraint {
public: // Aliases
    using Ptr = std::unique_ptr<ValueConstraint>;
    using Vec = std::vector<Ptr>;

public: // Methods
    ValueConstraint(const yy::location &loc, AST::Expression::Ptr value);
    ~ValueConstraint() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    AST::Expression::Ptr value_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_VALUE_CONSTRAINT_H
