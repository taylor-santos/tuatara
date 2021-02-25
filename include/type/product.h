#ifndef TYPE_PRODUCT_H
#define TYPE_PRODUCT_H

#include <vector>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Product : public Type {
public: // Methods
    Product(yy::location loc, std::vector<std::reference_wrapper<Type>> types);
    ~Product() override;

    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other, Context &ctx) const override;
    bool                             isSupertype(const Type &other, Context &ctx) const override;

protected: // Methods
    bool isSupertype(const class Product &other, Context &ctx) const override;

private: // Fields
    std::vector<std::reference_wrapper<Type>> types_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

class ManagedProduct final : public Product {
public: // Methods
    ManagedProduct(yy::location loc, std::vector<std::unique_ptr<Type>> types);
    ~ManagedProduct() override;

private: // Fields
    std::vector<std::unique_ptr<Type>> types_;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
