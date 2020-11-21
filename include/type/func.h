#ifndef FUNC_H
#define FUNC_H

#include <memory>
#include <optional>

#include "type/type.h"

namespace TypeChecker {

class Func : public Type {
private:
    std::optional<std::shared_ptr<Type>> arg_type;
    std::optional<std::shared_ptr<Type>> ret_type;

protected:
    void json(std::ostream &os) const override;

public:
    Func(
        yy::location                         loc,
        std::optional<std::shared_ptr<Type>> arg_type,
        std::optional<std::shared_ptr<Type>> ret_type);
    ~Func() override = default;
};

} // namespace TypeChecker

#endif // FUNC_H
