#ifndef PATTERN_WILDCARD_H
#define PATTERN_WILDCARD_H

#include "pattern/pattern.h"

#include "type/unit.h"

namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Wildcard : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Wildcard>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Wildcard(const yy::location &loc);
    ~Wildcard() override = default;
    [[nodiscard]] const std::string &getNodeName() const override;
    TypeChecker::Type &              getTypeImpl(TypeChecker::Context &ctx) override;

private: // Methods
    void json(std::ostream &os) const override;

private: // Fields
    TypeChecker::Unit type_;
};

} // namespace Pattern

#endif // PATTERN_WILDCARD_H
