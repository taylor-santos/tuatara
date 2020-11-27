#ifndef BOOL_H
#define BOOL_H

#include "expression.h"

namespace AST {

class Bool final : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Bool>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Bool(const yy::location &loc, bool value);
    ~Bool() override = default;

private: // Fields
    bool value;

private: // Methods
    void json(std::ostream &os) const override;
};

} // namespace AST

#endif // BOOL_H
