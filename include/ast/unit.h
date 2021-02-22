#ifndef AST_UNIT_H
#define AST_UNIT_H

#include "ast/lvalue.h"

namespace AST {

class Unit final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Unit>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Unit(const yy::location &loc);
    ~Unit() override = default;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_UNIT_H
