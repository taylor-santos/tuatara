#ifndef AST_CALL_H
#define AST_CALL_H

#include "ast/lvalue.h"

#include "type/type.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {
class Node;

class Call final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Call>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Call(const yy::location &loc, Expression::Ptr func, Expression::Ptr arg);
    ~Call() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr func_;
    Expression::Ptr arg_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_CALL_H
