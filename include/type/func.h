#ifndef FUNC_H
#define FUNC_H

#include <memory>
#include <optional>

#include "type/type.h"

namespace TypeChecker {

class Func final : public Type {
public: // Methods
    Func(
        yy::location                         loc,
        std::optional<std::shared_ptr<Type>> arg_type,
        std::optional<std::shared_ptr<Type>> ret_type);
    ~Func() override = default;

private: // Fields
    std::optional<std::shared_ptr<Type>> arg_type;
    std::optional<std::shared_ptr<Type>> ret_type;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // FUNC_H
