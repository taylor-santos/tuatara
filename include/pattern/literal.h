#ifndef PATTERN_LITERAL_H
#define PATTERN_LITERAL_H

#include "pattern/pattern.h"

namespace AST {
class Literal;
} // namespace AST
namespace yy {
class location;
} // namespace yy

namespace Pattern {

class Literal final : public Pattern {
public: // Methods
    explicit Literal(const yy::location &loc, std::unique_ptr<AST::Literal> literal);
    ~Literal() override;

    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::unique_ptr<AST::Literal> literal_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_LITERAL_H
