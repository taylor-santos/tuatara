#ifndef AST_INDEX_H
#define AST_INDEX_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
}  // namespace TypeChecker
namespace yy {
class location;
}  // namespace yy

namespace AST {
class Node;

class Index final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Index>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Index(const yy::location &loc, Expression::Ptr expr, Expression::Ptr index);
    ~Index() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    Expression::Ptr expr_;
    Expression::Ptr index_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_INDEX_H
