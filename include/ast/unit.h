#ifndef AST_UNIT_H
#define AST_UNIT_H

#include "ast/lvalue.h"

#include "type/unit.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Unit final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Unit>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Unit(const yy::location &loc);
    ~Unit() override = default;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
    TypeChecker::Unit  myType_;
};

} // namespace AST

#endif // AST_UNIT_H
