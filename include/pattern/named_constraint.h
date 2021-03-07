#ifndef PATTERN_NAMED_CONSTRAINT_H
#define PATTERN_NAMED_CONSTRAINT_H

#include "pattern/constraint.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

class NamedConstraint final : public Pattern {
public: // Methods
    explicit NamedConstraint(
        const yy::location &        loc,
        std::string                 name,
        std::unique_ptr<Constraint> constraint);
    ~NamedConstraint() override;

    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;
    const std::string *
    getName() override;

private: // Fields
    std::string                 name_;
    std::unique_ptr<Constraint> constraint_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_NAMED_CONSTRAINT_H
