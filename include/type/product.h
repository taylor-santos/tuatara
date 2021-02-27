#ifndef TYPE_PRODUCT_H
#define TYPE_PRODUCT_H

#include <vector>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Product final : public Type {
public: // Methods
    Product(yy::location loc, std::vector<std::shared_ptr<Type>> types);
    ~Product() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    void pretty(std::ostream &out, bool mod) const override;
    bool isSubtype(const Type &other, Context &ctx) const override;

private: // Fields
    std::vector<std::shared_ptr<Type>> types_;

private: // Methods
    void                  json(std::ostream &os) const override;
    void                  verifyImpl(Context &ctx) override;
    std::shared_ptr<Type> simplify(Context &ctx) override;
    bool                  isSuperImpl(const class Product &other, Context &ctx) const override;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
