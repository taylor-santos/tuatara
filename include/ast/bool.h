#ifndef AST_BOOL_H
#define AST_BOOL_H

#include "ast/literal.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Bool final : public Literal {
public: // Methods
    Bool(const yy::location &loc, bool value);
    ~Bool() override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    bool value_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_BOOL_H
