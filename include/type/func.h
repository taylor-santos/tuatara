#ifndef TYPE_FUNC_H
#define TYPE_FUNC_H

#include "type/type.h"

namespace AST {
class Expression;
} // namespace AST
namespace yy {
class location;
} // namespace yy

namespace TypeChecker {

class Context;

class Func final : public Type {
public: // Methods
    Func(yy::location loc, std::unique_ptr<Type> argType, std::unique_ptr<Type> retType);
    ~Func() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             isSubtype(const Type &other) const override;
    bool                             isSupertype(const Type &other) const override;
    Type &                           callAsFunc(Context &ctx, AST::Expression &arg) override;

protected: // Methods
    bool isSupertype(const Func &other) const override;

private: // Fields
    std::unique_ptr<Type> argType_;
    std::unique_ptr<Type> retType_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_FUNC_H
