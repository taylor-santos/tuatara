#ifndef AST_TUPLE_H
#define AST_TUPLE_H

#include <vector>

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Tuple final : public LValue {
public: // Methods
    Tuple(const yy::location &loc, std::vector<std::unique_ptr<Expression>> exprs);
    ~Tuple() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::vector<std::unique_ptr<Expression>> exprs_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_TUPLE_H
