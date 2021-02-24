#ifndef PATTERN_TYPE_CONSTRAINT_H
#define PATTERN_TYPE_CONSTRAINT_H

#include "pattern/constraint.h"

#include "type/type.h"

namespace AST {
class Node;
} // namespace AST
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class TypeConstraint final : public Constraint {
public: // Aliases
    using Ptr = std::unique_ptr<TypeConstraint>;
    using Vec = std::vector<Ptr>;

public: // Methods
    TypeConstraint(const yy::location &loc, std::unique_ptr<TypeChecker::Type> type);
    ~TypeConstraint() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<TypeChecker::Type> type_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_TYPE_CONSTRAINT_H
