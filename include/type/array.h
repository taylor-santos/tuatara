#ifndef ARRAY_H
#define ARRAY_H

#include <memory>

#include "type/type.h"

namespace TypeChecker {

class Array : public Type {
protected:
    std::shared_ptr<Type> type;

protected:
    void json(std::ostream &os) const override;

public:
    Array(yy::location loc, std::shared_ptr<Type> type);
    ~Array() override = default;
};

} // namespace TypeChecker

#endif // ARRAY_H
