#ifndef PATTERN_LITERAL_H
#define PATTERN_LITERAL_H

#include "pattern/pattern.h"

namespace AST {
class Literal;
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

private: // Fields
    std::unique_ptr<AST::Literal> literal_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace Pattern

#endif // PATTERN_LITERAL_H
