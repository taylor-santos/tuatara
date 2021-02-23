#ifndef PATTERN_NAMED_CONSTRAINT_H
#define PATTERN_NAMED_CONSTRAINT_H

#include "pattern/constraint.h"

namespace Pattern {

class NamedConstraint : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<NamedConstraint>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit NamedConstraint(const yy::location &loc, std::string name, Constraint::Ptr constraint);
    ~NamedConstraint() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::string     name_;
    Constraint::Ptr constraint_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_NAMED_CONSTRAINT_H
