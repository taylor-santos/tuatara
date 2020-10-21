#ifndef STRING_H
#define STRING_H

#include "expression.h"

namespace AST {

class String final : public Expression {
private: // Fields
    std::string value;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    String(const yy::location &loc, std::string value);
    ~String() override = default;
};

} // namespace AST

#endif // STRING_H
