#ifndef TYPED_VARIABLE_H
#define TYPED_VARIABLE_H

#include "variable.h"
#include "type/type.h"

#include <memory>

namespace AST {

class TypedVariable final : public Variable {
private: // Fields
    std::shared_ptr<TypeChecker::Type> type;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    TypedVariable(
        const yy::location &               loc,
        std::string                        name,
        std::shared_ptr<TypeChecker::Type> type);
};

} // namespace AST

#endif // TYPED_VARIABLE_H
