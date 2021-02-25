#ifndef AST_CALL_H
#define AST_CALL_H

#include "ast/lvalue.h"

namespace TypeChecker {
class Context;
} // namespace TypeChecker
namespace yy {
class location;
} // namespace yy

namespace AST {

class Call final : public LValue {
public: // Methods
    Call(
        const yy::location &        loc,
        std::unique_ptr<Expression> func,
        std::unique_ptr<Expression> arg);
    ~Call() override;
    void walk(const std::function<void(const Node &)> &fn) const override;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    std::unique_ptr<Expression> func_;
    std::unique_ptr<Expression> arg_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_CALL_H
