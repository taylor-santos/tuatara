#ifndef PATTERN_LITERAL_H
#define PATTERN_LITERAL_H

#include "ast/literal.h"

#include "pattern/pattern.h"

namespace Pattern {

class Literal final : public Pattern {
public: // Aliases
    using Ptr = std::unique_ptr<Literal>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Literal(const yy::location &loc, AST::Literal::Ptr literal);
    ~Literal() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    AST::Literal::Ptr literal_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_LITERAL_H
