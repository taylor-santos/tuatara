#ifndef TYPE_TUPLE_H
#define TYPE_TUPLE_H

#include "type/type.h"

namespace TypeChecker {

class Tuple final : public Type {
public: // Aliases
    using Ptr = std::shared_ptr<Tuple>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Tuple(yy::location loc, std::vector<Type::Ptr> types);
    ~Tuple() override = default;

private: // Fields
    std::vector<Type::Ptr> types;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_TUPLE_H
