#ifndef PATTERN_TYPE_CONSTRAINT_H
#define PATTERN_TYPE_CONSTRAINT_H

#include "pattern/constraint.h"

#include "type/type.h"

namespace Pattern {

class TypeConstraint final : public Constraint {
public: // Aliases
    using Ptr = std::unique_ptr<TypeConstraint>;
    using Vec = std::vector<Ptr>;

public: // Methods
    TypeConstraint(const yy::location &loc, TypeChecker::Type::Ptr type);
    ~TypeConstraint() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    TypeChecker::Type::Ptr type_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_TYPE_CONSTRAINT_H
