#ifndef PATTERN_TUPLE_H
#define PATTERN_TUPLE_H

#include <optional>
#include <vector>

#include "pattern/pattern.h"

namespace TypeChecker {
class Product;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Tuple final : public Pattern {
public: // Methods
    Tuple(const yy::location &loc, std::vector<std::unique_ptr<Pattern>> patterns);
    ~Tuple() override;

    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::vector<std::unique_ptr<Pattern>> patterns_;
    std::shared_ptr<TypeChecker::Product> type_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_TUPLE_H
