#ifndef AST_FLOAT_H
#define AST_FLOAT_H

#include "ast/literal.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Float final : public Literal {
public: // Methods
    Float(const yy::location &loc, double value);
    ~Float() override;
    [[nodiscard]] const std::string &
    getNodeName() const override;

private: // Fields
    double value_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_FLOAT_H
