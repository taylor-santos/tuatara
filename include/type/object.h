#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H

#include <optional>

#include "type/class.h"
#include "type/type.h"

namespace TypeChecker {

class Object final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Object>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Object(yy::location loc, std::string className);
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Object &other) const override;

private: // Fields
    std::string                                  className_;
    std::optional<std::reference_wrapper<Class>> ofClass_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_OBJECT_H
