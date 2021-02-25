#ifndef TYPE_SUM_H
#define TYPE_SUM_H

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Sum final : public Type {
public: // Fields
    Sum(yy::location loc, std::vector<std::unique_ptr<Type>> types);
    ~Sum() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other) const override;
    bool                             isSupertype(const Type &other) const override;

private: // Fields
    std::vector<std::unique_ptr<Type>> types_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
