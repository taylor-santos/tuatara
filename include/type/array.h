#ifndef ARRAY_H
#define ARRAY_H

#include <memory>

#include "type/type.h"

namespace TypeChecker {

class Array final : public Type {
public: // Methods
    Array(yy::location loc, std::shared_ptr<Type> type);
    ~Array() override = default;

private: // Fields
    std::shared_ptr<Type> type;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // ARRAY_H
