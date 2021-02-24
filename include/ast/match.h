#ifndef AST_MATCH_H
#define AST_MATCH_H

#include "ast/expression.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace Pattern {
class Pattern;
}

namespace AST {
class Node;

class Match final : public Expression {
public: // Aliases
    using Ptr  = std::unique_ptr<Match>;
    using Vec  = std::vector<Ptr>;
    using Case = std::pair<std::unique_ptr<Pattern::Pattern>, AST::Expression::Ptr>;

public: // Methods
    Match(const yy::location &loc, Expression::Ptr value, std::vector<Case> cases);
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr   value_;
    std::vector<Case> cases_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_MATCH_H
