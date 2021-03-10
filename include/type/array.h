#ifndef TYPE_ARRAY_H
#define TYPE_ARRAY_H

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Array final : public Type {
public: // Methods
    Array(yy::location loc, std::shared_ptr<Type> type);
    ~Array() override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    void
    walk(const std::function<void(const Node &)> &fn) const override;
    void
    pretty(std::ostream &out, bool mod) const override;
    bool
    isSubtype(const Type &other, const Context &ctx) const override;
    std::shared_ptr<Type>
    indexAsArray(const Type &arg, const yy::location &loc, Context &ctx) override;
    std::shared_ptr<Type>
    clone(const yy::location &loc) const override;

private: // Fields
    std::shared_ptr<Type> type_;

private: // Methods
    void
    json(std::ostream &os) const override;
    void
    verifyImpl(Context &ctx) override;
    std::shared_ptr<Type>
    simplify(Context &ctx) override;
    bool
    isSuperImpl(const class Array &other, const Context &ctx) const override;
    void
    updateWith(const Type &other) override;
};

} // namespace TypeChecker

#endif // TYPE_ARRAY_H
