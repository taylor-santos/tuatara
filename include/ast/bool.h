#ifndef AST_BOOL_H
#define AST_BOOL_H

#include "ast/literal.h"

namespace AST {

class Bool final : public Literal {
public: // Aliases
    using Ptr = std::unique_ptr<Bool>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Bool(const yy::location &loc, bool value);
    ~Bool() override = default;
    [[nodiscard]] const std::string &getTypeName() const override;

private: // Fields
    bool value_;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // AST_BOOL_H
