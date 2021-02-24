#ifndef TYPE_SUM_H
#define TYPE_SUM_H

#include "type/type.h"

namespace AST {
class Node;
}  // namespace AST
namespace yy {
class location;
}  // namespace yy

namespace TypeChecker {
class Context;

class Sum final : public Type {
public: // Aliases
    using Ptr = std::unique_ptr<Sum>;
    using Vec = std::vector<Ptr>;

public: // Fields
    Sum(yy::location loc, Type::Vec types);
    ~Sum() override = default;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;
    void                             pretty(std::ostream &out, bool mod) const override;
    bool                             operator<=(const Type &other) const override;

protected: // Methods
    bool operator>=(const Type &other) const override;

private: // Fields
    Type::Vec types_;

private: // Methods
    void json(std::ostream &os) const override;
    void verifyImpl(Context &ctx) override;
};

} // namespace TypeChecker

#endif // TYPE_SUM_H
