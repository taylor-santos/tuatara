#ifndef TYPE_SUM_H
#define TYPE_SUM_H

#include "type/type.h"

namespace TypeChecker {

class Sum final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Sum>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Sum(yy::location loc, Type::Vec types);
    ~Sum() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Type::Vec types_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
