#ifndef TYPE_FUNC_H
#define TYPE_FUNC_H

#include "type/type.h"

#include <optional>

namespace TypeChecker {

class Func final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Func>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Func(yy::location loc, std::optional<Type::Ptr> argType, std::optional<Type::Ptr> retType);
    ~Func() override = default;

private: // Fields
    std::optional<Type::Ptr> argType;
    std::optional<Type::Ptr> retType;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_FUNC_H
