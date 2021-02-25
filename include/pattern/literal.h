#ifndef PATTERN_LITERAL_H
#define PATTERN_LITERAL_H

#include "ast/literal.h"

#include "pattern/pattern.h"

namespace yy {
class location;
} // namespace yy

namespace AST {
class Node;
}

namespace Pattern {

class Literal final : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Literal>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Literal(const yy::location &loc, std::unique_ptr<AST::Literal> literal);
    ~Literal() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    TypeChecker::Type &              getTypeImpl(TypeChecker::Context &ctx) override;

private: // Fields
    std::unique_ptr<AST::Literal> literal_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_LITERAL_H
