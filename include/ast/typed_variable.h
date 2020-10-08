#ifndef TYPED_VARIABLE_H
#define TYPED_VARIABLE_H

#include "variable.h"
#include "type/type.h"

#include <memory>

namespace AST {

class TypedVariable final : public Variable {
protected:
    std::shared_ptr<TypeChecker::Type> type;

protected:
    void json(std::ostream &os) const override;

public:
    TypedVariable(
        const yy::location &               loc,
        std::string                        name,
        std::shared_ptr<TypeChecker::Type> type);
    ~TypedVariable() override = default;
};

} // namespace AST

#endif // TYPED_VARIABLE_H
