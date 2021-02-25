#ifndef TYPE_PRODUCT_H
#define TYPE_PRODUCT_H

#include "type/type.h"

namespace AST {
class Node;
} // namespace AST
namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Product : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Product>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Product(yy::location loc, std::vector<std::reference_wrapper<Type>> types);
    ~Product() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Product &other) const override;

private: // Fields
    std::vector<std::reference_wrapper<Type>> types_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

class ManagedProduct final : public Product {
public: // Aliases
    using Ptr = std::unique_ptr<ManagedProduct>;
    using Vec = std::vector<Ptr>;

public: // Methods
    ManagedProduct(yy::location loc, std::vector<Type::Ptr> types);
    ~ManagedProduct() override = default;

private: // Fields
    std::vector<Type::Ptr> types_;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
