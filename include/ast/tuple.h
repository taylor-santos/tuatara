#ifndef AST_TUPLE_H
#define AST_TUPLE_H

#include <optional>

#include "ast/lvalue.h"

namespace AST {

class Tuple final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Tuple>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Tuple(const yy::location &loc, Expression::Vec exprs);
    ~Tuple() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Expression::Vec exprs_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_TUPLE_H
