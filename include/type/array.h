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
    Array(yy::location loc, std::unique_ptr<Type> type);
    ~Array() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other, Context &ctx) const override;
    bool                             isSupertype(const Type &other, Context &ctx) const override;

protected: // Methods
    bool isSupertype(const class Array &other, Context &ctx) const override;

private: // Fields
    std::unique_ptr<Type> type_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_ARRAY_H
