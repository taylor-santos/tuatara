#ifndef TYPE_TYPE_H
#define TYPE_TYPE_H

#include "location.hh"

#include <memory>
#include <vector>

namespace TypeChecker {

class Type {
public: // Aliases
    using Ptr   = std::unique_ptr<Type>;
    using Vec   = std::vector<Ptr>;
    using Named = std::pair<std::string, Ptr>;

public: // Methods
    virtual ~Type() = default;
    friend std::ostream &operator<<(std::ostream &os, const Type &type);

protected: // Methods
    explicit Type(yy::location loc);
    virtual void json(std::ostream &os) const = 0;

    [[nodiscard]] const yy::location &getLoc() const;

private: // Fields
    yy::location loc;
};

} // namespace TypeChecker

#endif // TYPE_TYPE_H
