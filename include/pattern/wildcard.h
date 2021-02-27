#ifndef PATTERN_WILDCARD_H
#define PATTERN_WILDCARD_H

#include "pattern/pattern.h"

namespace TypeChecker {
class Unit;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Wildcard : public Pattern {
public: // Methods
    explicit Wildcard(const yy::location &loc);
    ~Wildcard() override;
    [[nodiscard]] const std::string &  getNodeName() const override;
    std::shared_ptr<TypeChecker::Type> getTypeImpl(TypeChecker::Context &ctx) override;

private: // Methods
    void json(std::ostream &os) const override;

private: // Fields
    std::shared_ptr<TypeChecker::Unit> type_;
};

} // namespace Pattern

#endif // PATTERN_WILDCARD_H
