#ifndef TYPE_MAYBE_H
#define TYPE_MAYBE_H

#include "type/type.h"

namespace TypeChecker {

class Maybe final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Maybe>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Maybe(yy::location loc, Type::Ptr type);
    ~Maybe() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Type::Ptr type_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_MAYBE_H
