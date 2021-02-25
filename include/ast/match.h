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

class Match final : public Expression {
public: // Aliases
    using Case = std::pair<std::unique_ptr<Pattern::Pattern>, std::unique_ptr<AST::Expression>>;

public: // Methods
    Match(const yy::location &loc, std::unique_ptr<Expression> value, std::vector<Case> cases);
    ~Match() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> value_;
    std::vector<Case>           cases_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_MATCH_H
