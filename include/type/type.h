#ifndef TYPE_H
#define TYPE_H

#include "location.hh"

// Namespace and class must be forward declared for operator<< to work properly
namespace TypeChecker {
class Type;
}
std::ostream &operator<<(std::ostream &os, const TypeChecker::Type &type);

namespace TypeChecker {

class Type {
protected:
    yy::location loc;

protected:
    explicit Type(yy::location loc);
    virtual void json(std::ostream &os) const = 0;

public:
    virtual ~Type() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const Type &type);
};

} // namespace TypeChecker

#endif // TYPE_H
