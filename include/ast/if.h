#ifndef AST_IF_H
#define AST_IF_H

#include <optional>

#include "ast/block.h"

namespace AST {

class If : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<If>;
    using Vec = std::vector<Ptr>;

public: // Methods
    If(const yy::location &loc, Expression::Ptr cond, Block::Ptr block);
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

protected: // Methods
    [[nodiscard]] const Expression &getCond() const;
    [[nodiscard]] const Expression &getBlock() const;

private: // Fields
    Expression::Ptr cond_;
    Block::Ptr      block_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_IF_H
