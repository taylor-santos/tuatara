#ifndef AST_UNIT_H
#define AST_UNIT_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Unit;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Unit final : public LValue {
public: // Methods
    explicit Unit(const yy::location &loc);
    ~Unit() override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Methods
    void                               json(std::ostream &os) const override;
    std::shared_ptr<TypeChecker::Type> getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_UNIT_H
