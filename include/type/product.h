#ifndef TYPE_PRODUCT_H
#define TYPE_PRODUCT_H

#include "type/type.h"

namespace TypeChecker {

class Product final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Product>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Product(yy::location loc, std::vector<Type::Ptr> types);
    ~Product() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Product &other) const override;

private: // Fields
    std::vector<Type::Ptr> types_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
