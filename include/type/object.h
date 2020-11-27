#ifndef OBJECT_H
#define OBJECT_H

#include "type/type.h"

namespace TypeChecker {

class Object final : public Type {
public: // Aliases
    using Ptr = std::shared_ptr<Object>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Object(yy::location loc, std::string class_name);
    ~Object() override = default;

private: // Fields
    std::string class_name;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // OBJECT_H
