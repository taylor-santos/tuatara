#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H

#include <optional>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Class;
class Context;

class Object final : public Type {
public: // Methods
    Object(yy::location loc, std::string className);
    ~Object() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other) const override;
    bool                             isSupertype(const Type &other) const override;
    [[nodiscard]] const Class &      getClass() const;

protected: // Methods
    bool isSupertype(const Object &other) const override;

private: // Fields
    std::string                                  className_;
    std::optional<std::reference_wrapper<Class>> ofClass_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_OBJECT_H
