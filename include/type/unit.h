#ifndef TYPE_UNIT_H
#define TYPE_UNIT_H

#include "type/type.h"

namespace TypeChecker {

class Unit final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Unit>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Unit(yy::location loc);
    ~Unit() override = default;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_UNIT_H
