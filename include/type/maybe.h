#ifndef TYPE_MAYBE_H
#define TYPE_MAYBE_H

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Maybe final : public Type {
public: // Methods
    Maybe(yy::location loc, std::unique_ptr<Type> type);
    ~Maybe() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other) const override;
    bool                             isSupertype(const Type &other) const override;

protected: // Methods
    bool isSupertype(const Maybe &other) const override;

private: // Fields
    std::unique_ptr<Type> type_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_MAYBE_H
