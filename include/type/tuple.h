#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <memory>

#include "type/type.h"

namespace TypeChecker {

class Tuple final : public Type {
public: // Fields
    Tuple(yy::location loc, std::vector<Type::Ptr> types);
    ~Tuple() override = default;

private: // Fields
    std::vector<Type::Ptr> types;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TUPLE_H
