#ifndef AST_VARIABLE_H
#define AST_VARIABLE_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
class Type;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Variable final : public LValue {
public: // Methods
    Variable(const yy::location &loc, std::string name);
    ~Variable() override;

    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::string name_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_VARIABLE_H
