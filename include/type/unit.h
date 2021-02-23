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
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Type &other) const override;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_UNIT_H
