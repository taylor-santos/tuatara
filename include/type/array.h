#ifndef ARRAY_H
#define ARRAY_H

#include "type/type.h"

namespace TypeChecker {

class Array final : public Type {
public: // Aliases
    using Ptr = std::shared_ptr<Array>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Array(yy::location loc, Type::Ptr type);
    ~Array() override = default;

private: // Fields
    Type::Ptr type;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // ARRAY_H
