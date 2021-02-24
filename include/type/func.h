#ifndef TYPE_FUNC_H
#define TYPE_FUNC_H

#include "type/type.h"

namespace AST {
class Expression;
class Node;
}  // namespace AST
namespace yy {
class location;
}  // namespace yy

namespace TypeChecker {

class Context;

class Func final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Func>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Func(yy::location loc, Type::Ptr argType, Type::Ptr retType);
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;
    Type &                           callAsFunc(Context &ctx, AST::Expression &arg) override;

protected: // Methods
    bool operator>=(const Func &other) const override;

private: // Fields
    Type::Ptr argType_;
    Type::Ptr retType_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_FUNC_H
