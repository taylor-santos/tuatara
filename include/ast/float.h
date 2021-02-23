#ifndef AST_FLOAT_H
#define AST_FLOAT_H

#include "ast/literal.h"

namespace AST {

class Float final : public Literal {
public: // Aliases
    using Ptr = std::unique_ptr<Float>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Float(const yy::location &loc, double value);
    ~Float() override = default;
    [[nodiscard]] const std::string &getNodeName() const override;

private: // Fields
    double value_;

private: // Methods
    void               json(std::ostream &os) const override;
    TypeChecker::Type &getTypeImpl(TypeChecker::Context &ctx) override;
};

} // namespace AST

#endif // AST_FLOAT_H
