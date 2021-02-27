#ifndef TYPE_SUM_H
#define TYPE_SUM_H

#include <vector>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Sum final : public Type {
public: // Fields
    Sum(yy::location loc, std::vector<std::shared_ptr<Type>> types);
    ~Sum() override;
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
    bool                  isSuper(const Type &other, Context &ctx) const;
    bool                  isSuperImpl(const Array &other, Context &ctx) const override;
    bool                  isSuperImpl(const Func &other, Context &ctx) const override;
    bool                  isSuperImpl(const Maybe &other, Context &ctx) const override;
    bool                  isSuperImpl(const Object &other, Context &ctx) const override;
    bool                  isSuperImpl(const Product &other, Context &ctx) const override;
    bool                  isSuperImpl(const Sum &other, Context &ctx) const override;
    bool                  isSuperImpl(const Unit &other, Context &ctx) const override;
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
