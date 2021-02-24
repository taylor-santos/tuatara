#ifndef AST_TUPLE_H
#define AST_TUPLE_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {
class Node;

class Tuple final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Tuple>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Tuple(const yy::location &loc, Expression::Vec exprs);
    ~Tuple() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Vec exprs_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_TUPLE_H
