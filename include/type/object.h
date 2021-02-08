#ifndef TYPE_OBJECT_H
#define TYPE_OBJECT_H

#include "type/type.h"

namespace TypeChecker {

class Object final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Object>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Object(yy::location loc, std::string className);
    ~Object() override = default;

private: // Fields
    std::string className;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_OBJECT_H
