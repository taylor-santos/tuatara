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
    Product(yy::location loc, std::vector<std::unique_ptr<Type>> types);
    ~Product() override;

    void                             walk(const std::function<void(const Node &)> &fn) const final;
    [[nodiscard]] const std::string &getNodeName() const final;
    void                             pretty(std::ostream &out, bool mod) const final;
    bool                             isSubtype(const Type &other, Context &ctx) const final;

private: // Fields
    std::vector<std::unique_ptr<Type>>        ownedTypes_;
    std::vector<std::reference_wrapper<Type>> types_;

private: // Methods
    void json(std::ostream &os) const final;
    void verifyImpl(Context &ctx) final;
    bool isSuperImpl(const class Product &other, Context &ctx) const final;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
