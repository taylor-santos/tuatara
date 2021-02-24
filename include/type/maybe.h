#ifndef TYPE_MAYBE_H
#define TYPE_MAYBE_H

#include "type/type.h"

namespace AST {
class Node;
}  // namespace AST
namespace yy {
class location;
}  // namespace yy

namespace TypeChecker {
class Context;

class Maybe final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Maybe>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Maybe(yy::location loc, Type::Ptr type);
    ~Maybe() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Type &other) const override;
    bool operator>=(const Maybe &other) const override;

private: // Fields
    Type::Ptr type_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_MAYBE_H
