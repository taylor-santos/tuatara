#ifndef TYPE_MAYBE_H
#define TYPE_MAYBE_H

#include "type/type.h"

namespace yy {
class location;
} // namespace yy

namespace TypeChecker {
class Context;

class Maybe final : public Type {
public: // Methods
    Maybe(yy::location loc, std::shared_ptr<Type> type);
    ~Maybe() override;
    [[nodiscard]] const std::string &
    getNodeName() const override;
    void
    walk(const std::function<void(const Node &)> &fn) const override;
    void
    pretty(std::ostream &out, bool mod) const override;
    bool
    isSubtype(const Type &other, const Context &ctx) const override;
    std::shared_ptr<Type>
    callAsFunc(const Type &arg, const AST::Expression &call, Context &ctx) override;
    std::shared_ptr<Type>
    indexAsArray(AST::Expression &arg, const AST::Expression &index, Context &ctx) override;
    std::shared_ptr<Type>
    accessField(const std::string &field, const AST::Expression &access, Context &ctx) override;

private: // Fields
    std::shared_ptr<Type> type_;

private: // Methods
    void
    json(std::ostream &os) const override;
    void
    verifyImpl(Context &ctx) override;
    std::shared_ptr<Type>
    simplify(Context &ctx) override;
    bool
    isSuperImpl(const class Maybe &other, const Context &ctx) const override;
};

} // namespace TypeChecker

#endif // TYPE_MAYBE_H
