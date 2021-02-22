#ifndef AST_CALL_H
#define AST_CALL_H

#include <optional>

#include "ast/lvalue.h"

namespace AST {

class Call final : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Call>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Call(const yy::location &loc, Expression::Ptr func, std::optional<Expression::Ptr> arg);
    ~Call() override = default;
    void                             walk(const Func &fn) const override;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    Expression::Ptr                func_;
    std::optional<Expression::Ptr> arg_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_CALL_H
