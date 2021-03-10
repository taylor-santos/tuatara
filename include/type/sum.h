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
    Sum(yy::location                       loc,
        std::vector<std::shared_ptr<Type>> types,
        const std::shared_ptr<int> &       indexOverride);
    ~Sum() override;
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
    // Type checker will branch for each sub-type of a sum type, assuming that it takes on that
    // type. The indexOverride_ field encodes that assumption so if the same type is encountered
    // again it will maintain that assumption.
    std::shared_ptr<int> indexOverride_;

private: // Methods
    void
    json(std::ostream &os) const override;
    void
    verifyImpl(Context &ctx) override;
    std::shared_ptr<Type>
    simplify(Context &ctx) override;
    bool
    isSuper(const Type &other, const Context &ctx) const;
    bool
    isSuperImpl(const Array &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Func &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Maybe &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Object &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Product &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Sum &other, const Context &ctx) const override;
    bool
    isSuperImpl(const Unit &other, const Context &ctx) const override;
    void
    updateWith(const Type &other) override;
    void
    updateForImpl(Sum &other) const override;

private: // Classes
    // RAII setter for Sum::indexOverride_. Resets to -1 when the object goes out of scope.
    friend class IndexOverrider;
    class IndexOverrider {
    public: // Methods
        IndexOverrider(Sum &sum, int index);
        ~IndexOverrider();

    private: // Fields
        Sum &sum_;
    };
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
