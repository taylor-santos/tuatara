#ifndef OBJECT_H
#define OBJECT_H

#include "type/type.h"

namespace TypeChecker {

class Object : public Type {
protected:
    std::string class_name;

protected:
    void json(std::ostream &os) const override;

public:
    Object(yy::location loc, std::string class_name);
    ~Object() override = default;
};

} // namespace TypeChecker

#endif // OBJECT_H
