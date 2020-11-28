#ifndef TYPE_TYPE_H
#define TYPE_TYPE_H

#include "location.hh"

#include <memory>
#include <vector>

// Namespace and class must be forward declared for operator<< to work properly
namespace TypeChecker {
class Type;
}
std::ostream &operator<<(std::ostream &os, const TypeChecker::Type &type);

namespace TypeChecker {

class Type {
public: // Aliases
    using Ptr = std::shared_ptr<Type>;
    using Vec = std::vector<Ptr>;

public: // Methods
    virtual ~Type() = default;
    friend std::ostream & ::operator<<(std::ostream &os, const Type &type);

protected: // Methods
    explicit Type(yy::location loc);
    const yy::location &getLoc() const;
    virtual void        json(std::ostream &os) const = 0;

private: // Fields
    yy::location loc;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
