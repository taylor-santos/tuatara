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
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Type::Ptr type_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_ARRAY_H
