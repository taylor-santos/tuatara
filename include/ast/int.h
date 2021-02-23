#ifndef AST_INT_H
#define AST_INT_H

#include "ast/literal.h"

namespace AST {

class Int final : public Literal {
public: // Aliases
    using Ptr = std::unique_ptr<Int>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Int(const yy::location &loc, int64_t value);
    ~Int() override = default;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    int64_t value_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_INT_H
