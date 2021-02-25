#ifndef PATTERN_TYPE_CONSTRAINT_H
#define PATTERN_TYPE_CONSTRAINT_H

#include "pattern/constraint.h"

namespace TypeChecker {
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class TypeConstraint final : public Constraint {
public: // Methods
    TypeConstraint(const yy::location &loc, std::unique_ptr<TypeChecker::Type> type);
    ~TypeConstraint() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    TypeChecker::Type &              getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::unique_ptr<TypeChecker::Type> type_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_TYPE_CONSTRAINT_H
