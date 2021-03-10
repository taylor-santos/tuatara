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
    [[nodiscard]] const std::string &
    getNodeName() const override;
    void
    walk(const std::function<void(const Node &)> &fn) const override;
    void
    pretty(std::ostream &out, bool mod) const override;
    bool
    isSubtype(const Type &other, const Context &ctx) const override;
    std::shared_ptr<Type>
    callAsFunc(const Type &arg, const yy::location &loc, Context &ctx) override;
    std::shared_ptr<Type>
    indexAsArray(const Type &arg, const yy::location &loc, Context &ctx) override;
    std::shared_ptr<Type>
    accessField(const std::string &field, const yy::location &loc, Context &ctx) override;
    std::shared_ptr<Type>
    clone(const yy::location &loc) const override;

private: // Fields
    std::vector<std::shared_ptr<Type>> types_;

private: // Methods
    void
    json(std::ostream &os) const override;
    void
    verifyImpl(Context &ctx) override;
    std::shared_ptr<Type>
    simplify(Context &ctx) override;
    bool
    isSuperImpl(const class Product &other, const Context &ctx) const override;
    void
    updateWith(const Type &other) override;
};

} // namespace TypeChecker

#endif // TYPE_PRODUCT_H
