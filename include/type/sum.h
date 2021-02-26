#ifndef TYPE_SUM_H
#define TYPE_SUM_H

#include <vector>

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Sum : public Type {
public: // Fields
    Sum(yy::location loc, std::vector<std::reference_wrapper<Type>> types);
    Sum(yy::location loc, std::vector<std::unique_ptr<Type>> types);
    ~Sum() override;

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
    bool isSuper(const Type &other, Context &ctx) const;
    bool isSuperImpl(const Array &other, Context &ctx) const final;
    bool isSuperImpl(const Func &other, Context &ctx) const final;
    bool isSuperImpl(const Maybe &other, Context &ctx) const final;
    bool isSuperImpl(const Object &other, Context &ctx) const final;
    bool isSuperImpl(const Product &other, Context &ctx) const final;
    bool isSuperImpl(const Sum &other, Context &ctx) const final;
    bool isSuperImpl(const Unit &other, Context &ctx) const final;
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
