#ifndef TUPLE_H
#define TUPLE_H

#include <vector>
#include <memory>

#include "type/type.h"

namespace TypeChecker {

class Tuple : public Type {
private:
    std::vector<std::shared_ptr<Type>> types;

protected:
    void json(std::ostream &os) const override;

public:
    Tuple(yy::location loc, std::vector<std::shared_ptr<Type>> types);
    ~Tuple() override = default;
};

} // namespace TypeChecker

#endif // TUPLE_H
