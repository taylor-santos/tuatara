#ifndef TYPE_ARRAY_H
#define TYPE_ARRAY_H

#include "type/type.h"

namespace TypeChecker {

class Array final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Array>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Array(yy::location loc, Type::Ptr type);
    ~Array() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Array &other) const override;

private: // Fields
    Type::Ptr type_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_ARRAY_H
