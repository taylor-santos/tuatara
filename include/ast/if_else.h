#ifndef AST_IF_ELSE_H
#define AST_IF_ELSE_H

#include "ast/if.h"

namespace AST {

class IfElse final : public If {
public: // Aliases
    using Ptr = std::unique_ptr<IfElse>;
    using Vec = std::vector<Ptr>;

public: // Methods
    IfElse(const yy::location &loc, Expression::Ptr cond, Block::Ptr block, Block::Ptr elseBlock);
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Block::Ptr elseBlock_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_IF_ELSE_H
