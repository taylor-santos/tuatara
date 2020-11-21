#ifndef CALL_H
#define CALL_H

#include "lvalue.h"

#include <memory>
#include <vector>

namespace AST {

class Call final : public LValue {
private: // Fields
    std::unique_ptr<Expression>              function;
    std::vector<std::unique_ptr<Expression>> args;

private: // Methods
    void json(std::ostream &os) const override;

public: // Methods
    Call(
        const yy::location &                     loc,
        std::unique_ptr<Expression>              function,
        std::vector<std::unique_ptr<Expression>> args);
    ~Call() override = default;
};

} // namespace AST

#endif // CALL_H
