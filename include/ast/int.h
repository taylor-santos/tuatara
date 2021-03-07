#ifndef AST_INT_H
#define AST_INT_H

#include "ast/literal.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Int final : public Literal {
public: // Methods
    Int(const yy::location &loc, int64_t value);
    ~Int() override;

    [[nodiscard]] const std::string &
    getNodeName() const override;

private: // Fields
    int64_t value_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_INT_H
