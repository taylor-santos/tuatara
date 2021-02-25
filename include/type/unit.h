#ifndef TYPE_UNIT_H
#define TYPE_UNIT_H

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Unit final : public Type {
public: // Methods
    explicit Unit(yy::location loc);
    ~Unit() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other, Context &ctx) const override;
    bool                             isSupertype(const Type &other, Context &ctx) const override;

protected: // Methods
private:   // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_UNIT_H
