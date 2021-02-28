#ifndef TYPE_FUNC_H
#define TYPE_FUNC_H

#include <common.h>

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
    Func(yy::location loc, std::shared_ptr<Type> argType, std::shared_ptr<Type> retType);
    ~Func() override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    void pretty(std::ostream &out, bool mod) const override;
    std::shared_ptr<Type>
         callAsFunc(Context &ctx, AST::Expression &arg, const AST::Call &call) override;
    bool isSubtype(const Type &other, const Context &ctx) const override;

private: // Fields
    std::shared_ptr<Type> argType_;
    std::shared_ptr<Type> retType_;

private: // Methods
    void                  json(std::ostream &os) const override;
    void                  verifyImpl(Context &ctx) override;
    std::shared_ptr<Type> simplify(Context &ctx) override;
    bool                  isSuperImpl(const class Func &other, const Context &ctx) const override;
};

} // namespace TypeChecker

#endif // TYPE_FUNC_H
