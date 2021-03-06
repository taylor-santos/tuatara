#ifndef TYPE_PRODUCT_H
#define TYPE_PRODUCT_H

#include "type/type.h"

namespace TypeChecker {

class Product final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Product>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Product(yy::location loc, std::vector<Type::Ptr> types);
    ~Product() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    std::vector<Type::Ptr> types_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
