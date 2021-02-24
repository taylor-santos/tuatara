#ifndef AST_OPERATOR_H
#define AST_OPERATOR_H

#include "ast/lvalue.h"

namespace yy {
class location;
} // namespace yy

namespace AST {

class Operator : public LValue {
public: // Aliases
    using Ptr = std::unique_ptr<Operator>;
    using Vec = std::vector<Ptr>;

public: // Methods
    Operator(const yy::location &loc, std::string op);
    ~Operator() override = default;

protected: // Methods
    [[nodiscard]] const std::string &getOp() const;

private: // Fields
    std::string op_;
};

} // namespace AST

#endif // AST_OPERATOR_H
