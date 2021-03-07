#ifndef AST_STRING_H
#define AST_STRING_H

#include "ast/literal.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class String final : public Literal {
public: // Methods
    String(const yy::location &loc, std::string value);
    ~String() override;

    [[nodiscard]] const std::string &
    getNodeName() const override;

private: // Fields
    std::string value_;

private: // Methods
    void
    json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_STRING_H
