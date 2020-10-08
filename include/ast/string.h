#ifndef STRING_H
#define STRING_H

#include "expression.h"

namespace AST {

class String final : public Expression {
private:
    std::string value;

protected:
    void json(std::ostream &os) const override;

public:
    String(const yy::location &loc, std::string value);
    ~String() override = default;
};

} // namespace AST

#endif // STRING_H
