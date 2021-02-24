#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "ast/expression.h"

#include "type/object.h"

namespace yy {
class location;
} // namespace yy

namespace AST {

class Literal : public Expression {
public: // Aliases
    using Ptr = std::unique_ptr<Literal>;
    using Vec = std::vector<Ptr>;

public: // Methods
    explicit Literal(const yy::location &loc, std::string className);
    ~Literal() override = default;

protected: // Methods
    [[nodiscard]] TypeChecker::Object &getMyType();

private: // Fields
    TypeChecker::Object myType_;
};

} // namespace AST

#endif // AST_LITERAL_H
