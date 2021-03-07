#ifndef AST_ARRAY_H
#define AST_ARRAY_H

#include <vector>

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
class Product;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Array final : public LValue {
public: // Methods
    Array(const yy::location &loc, std::vector<std::unique_ptr<Expression>> exprs);
    ~Array() override;

    void
    walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &
    getNodeName() const override;

private: // Fields
    std::vector<std::unique_ptr<Expression>> exprs_;

private: // Methods
    void
    json(std::ostream &os) const override;
    std::shared_ptr<TypeChecker::Type>
    getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_ARRAY_H
