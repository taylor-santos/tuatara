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
    [[nodiscard]] const std::string &
    getNodeName() const override;
    void
    pretty(std::ostream &out, bool mod) const override;
    bool
    isSubtype(const Type &other, const Context &ctx) const override;
    std::shared_ptr<Type>
    clone(const yy::location &loc) const override;

private: // Methods
    void
    json(std::ostream &os) const override;
    void
    verifyImpl(Context &ctx) override;
    bool
    isSuperImpl(const Array &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Func &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Maybe &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Object &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Product &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Sum &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Unit &other, const Context &ctx) const override;
    void
    updateWith(const Type &other) override;
};

} // namespace TypeChecker

#endif // TYPE_UNIT_H
